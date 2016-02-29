#ifndef	__BUZZER_H__
#define __BUZZER_H__

#define FQ 
#define PI 3.14 

#define NONE 	0

#define DOH 	523
#define DOH_S	554
#define LAE_P	554
#define LAE 	587
#define LAE_S	622
#define MI_P	622
#define MI 		659
#define PA 		699
#define PA_S	739
#define SOL_P	739
#define SOL 	784
#define SOL_S	830
#define RA_P	830
#define RA 		880
#define RA_S	932
#define SI_P	932
#define SI 		988

#define _DOH 	1046

#define _DOH_S	1108
#define _LAE_P	1108

#define _LAE 	1174

#define _LAE_S	1244
#define _MI_P   1244

#define _MI		1318
#define _PA		1396

#define _PA_S	1479
#define _SOL_P	1479

#define _SOL 	1567

#define _SOL_S	1661
#define _RA_P	1661

#define _RA		1760

#define _RA_S	1864
#define _SI_P	1864

#define _SI		1975


#define DEFAULT_LEN 200

void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);
void buzzer_set_frequency(int);
void buzzer_sine(int period);
void buzzer_play(int,int); 
void buzzer_delay(int );

#endif