# Arduino Entropy Tests

So, the question came up a few times within the week on /r/Arduino, and I decided to get at least a little rigorous about it.

I tested a few things, on a Lilypad Arduino, powered and attached to my computer via an FTDI adapter.

1. Unconnected ADC Pin (A1), with the pins in default condition
2. Same as 1, but with A0 and A2 driven HIGH
3. Same as 1, but with A0 and A2 driven LOW
4. Reading the internal temperature sensor, with the default ADC settings
5. Reading the internal temperature sensor with various prescaler settings

In terms of just reading the ADC and using that value to seed, it's generally not a good plan.

Here's 768 readings of an open ADC pin under three different conditions (1,2,3 above):

![Open ADC Pin](http://imgur.com/VuRICgO)

Now, this is not without entropy, but the values are rather consistent, only landing on a few code points.   Ideally, we want this graph to look something like this:

![Snow](http://imgur.com/TIe4b1d)

While it looks clumpy, that's what real random data looks like.

So, let's take the least significant bit of each reading, sum them together a bunch into an 8-bit integer.  It'll overflow, but that overflow is unpredictable.   

![Clumpy](http://imgur.com/AHFNd3A)

There's a good bit of dispersion here-- but the numbers are all landing between about 100 and 175, only covering about 30% of the whole range.   

So I'm a proponent of using the internal temperature sensor-- in theory the signal should be noisy in the LSB, which should give this technique a good run.

Here's the raw readings:

![A little bouncy](http://imgur.com/Sq62QTu)

The black line is the reading at the stock ADC frequency, the red is at a higher frequency.    While the signal shows a good bit of randomness, it **on average** is very flat.   When we sum this all together, we get this-- this is 1024 readings, LSbit summed):

![Ugh](http://imgur.com/Yhj3Oum)

While the black signal does cover the whole range, at any given moment, it's in a very tight clump.   There just isn't that much variation in the signal.  

### Herein commences a fair bit of nerdling around.

After some experimentation, the problem comes down to getting the values to overflow a *LOT* -- that is, you are trying to amplify the differences between a small number of readings, in such a way that the modulo of those differences is spread very wide.  

I had been taking a very small number of bits, and trying to construct a random number out of it.   One option would be to take many many many more readings, but it's possibly easier to just make sure they overflow a lot.   

That is, if I have the following readings: 371, 377, 382, 384, 386, 387, 377, 381, 378, 382, 377, 381.   

Summing just their LSbits gets us: 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, or about 6 (or exactly 50%).  Summing the numbers together gets us 4563, which modulo 256 is 211.   

Taking the next 12 numbers in the sequence, we have: 378, 382, 377, 380, 377, 379, 376, 377, 376, 377, 361, 382.

Summing their LSbits gets us 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, again about 6. Summing them together again gets us 4522, which modulo 256 is 170.   

This problem scales nicely-- by summing the whole bytes a bunch (I'm using 1024 reads cycled), you get very nice dispersion of values:

![Snow](http://imgur.com/TIe4b1d)

But unless you're doing cryptography, you don't need real random numbers, you just need to randomly seed the PRNG.  This is a good technique to generate random seeds.

Here's the final function I ended up with:

    uint8_t get_seed(int pin)
    {
      int x;
      uint8_t seed = 0;

      for(x = 0; x < 1024; x++)
      {
        seed += analogRead(pin);
      }

      return seed;

    }

You pass it the number of an unconnected analog pin.    It does not appear to be affected by the values of the adjacent pins, but can be affected by being connected (either in actuality or through a high-resistance link to another part of the circuit-- like a finger).   So it might be worth adding checks to the function to make sure the analogRead is pretty random.
