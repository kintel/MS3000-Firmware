#include <Base64.h>

/*
 Base64 Encode/Decode example
 
 Encodes the text "Hello world" to "SGVsbG8gd29ybGQA" and decodes "Zm9vYmFy" to "foobar"

 Created 29 April 2015
 by Nathan Friedly - http://nfriedly.com/
 
 This example code is in the public domain.

 */


void setup()
{
  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  msSystem.slogln("Base64 example");
  
  
  
  // encoding
  char input[] = "Hello world";
  int inputLen = sizeof(input);
  
  int encodedLen = base64_enc_len(inputLen);
  char encoded[encodedLen];
  
  msSystem.slogln(input); msSystem.slogln(" = ");
  
  // note input is consumed in this step: it will be empty afterwards
  base64_encode(encoded, input, inputLen); 
  
  msSystem.slogln(encoded);
  
  
  
  // decoding
  char input2[] = "Zm9vYmFy";
  int input2Len = sizeof(input2);
  
  int decodedLen = base64_dec_len(input2, input2Len);
  char decoded[decodedLen];
  
  base64_decode(decoded, input2, input2Len);
  
  msSystem.slogln(input2); msSystem.slogln(" = "); msSystem.slogln(decoded);
}


void loop()
{
  
}
