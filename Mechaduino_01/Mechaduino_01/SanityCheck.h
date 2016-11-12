
/**
   SanityCheck.h

   Test configuration values for errors at compile-time.
*/
#ifndef SANITYCHECK_H
#define SANITYCHECK_H


#if !defined(ARDUINO) || ARDUINO < 10600
#error Versions of Arduino IDE prior to 1.6.0 are no longer supported, please update your toolkit.
#endif

#ifndef _VARIANT_ARDUINO_ZERO_
#error Oops!  Make sure you have 'Arduino Zero (Native USB Port)' selected from the 'Tools -> Boards' menu.
#endif



#endif //SANITYCHECK_H
