// Defines for function numbers to be used in the function pointer array go here

#define FNCNR_LED 0
#define FNCNR_MOTORS 1

/*********************************
 *    Structs                    *
 *********************************/

struct data_packet {
  char funcnr;
  char data[7];
};

