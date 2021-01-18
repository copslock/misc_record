#Why MIN is 34 bits length

The Mobile Identification Number (MIN) is a number that uniquely identifies a mobile telephone subscriber.

MINs are 34-bits in length. The first 10 bits are sometimes known as MIN2, while the last 24 bits are referred to as MIN1. Together they are simply known as the MIN.

In the United States, the MIN is derived from the 10 digital decimal telephone number assigned to the handset. For the telephone number (303)866-1010, the area code (“303″) becomes the 10 bit MIN2 and the local portion of the telephone number (“866-1010″) becomes the 24 bit MIN1.

999 = 11 1110 0111 
MIN1.length = 10
999-9999 = 1001 1000 1001 0110 0111 1111
MIN2.length = 24

MIN.length = MIN1.length + MIN2.length = 10 + 24 = 34

Internationally, MINs are calculated in a different fashion. The three digit mobile carrier identification number becomes MIN2 and the local portion of the telephone number becomes MIN1. IFAST, the International Forum on ANSI-41 Standards Technology, assigns the mobile carrier identification numbers. A MIN in this format is known as an IRM (International Roaming MIN).

Unlike the IMEI, the MIN is not an attribute of the physical phone. The MIN is stored in a database that the cellular provider manages and can be changed at any time.
