#include "F:\DUT\S2\Architecture et programmation\Digicode\digicode.h"

#define c1h output_high(pin_d0)
#define c1l output_low(pin_d0)
#define c2h output_high(pin_d1)
#define c2l output_low(pin_d1)
#define c3h output_high(pin_d2)
#define c3l output_low(pin_d2)
#define alarme_on output_high(pin_c1)
#define alarme_off output_low(pin_c1)

#define l1 input(pin_d3)
#define l2 input(pin_d4)
#define l3 input(pin_d5)
#define l4 input(pin_d6)

#define porte1 input(pin_b1)
#define porte2 input(pin_b2)
#define fenetre1 input(pin_b3)
#define fenetre2 input(pin_b4)
#define fenetre3 input(pin_b5)
#define fenetre4 input(pin_b6)

int16 code,dixieme,seconde;
int1 alarme,sonne;

#int_TIMER1
void TIMER1_isr(void)
{
   set_timer1(3036);
   dixieme++;
   if(dixieme==10)
   {
      dixieme=0;
      seconde++;
   }
}

#int_EXT
void  EXT_isr(void) 
{
   c1h;c2l;c3l;
   delay_us(50);
   if(l1) {printf("1");code=code*10+1;}
   if(l2) {printf("4");code=code*10+4;}
   if(l3) {printf("7");code=code*10+7;}
   
   c2h;c1l;c3l;
   delay_us(50);
   if(l1) {printf("2");code=code*10+2;}
   if(l2) {printf("5");code=code*10+5;}
   if(l3) {printf("8");code=code*10+8;}
   if(l4) {printf("0");code=code*10;}
   
   c3h;c1l;c2l;
   delay_us(50);
   if(l1) {printf("3");code=code*10+3;}
   if(l2) {printf("6");code=code*10+6;}
   if(l3) {printf("9");code=code*10+9;}
   if(l4) {printf("\n\r");
      if(code==33)
      {
         printf("Alarme activee\n\r");
         seconde=0;
         alarme=1;
         code=0;
      }
      if(code==1664)
      {
         printf("Alarme desactivee\n\r");
         alarme=0;
         code=0;
      }   
      if(code!=33 && code!=1664 && code!=0)
      {
         printf("Saisie incorrecte!\n\r");
         code=0;
      } 
   }   
   c1h;c2h;c3h;
   delay_us(50);
}

void main()
{

   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_CLOCK_DIV_2);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
   setup_timer_2(T2_DISABLED,0,1);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(INT_EXT);
   enable_interrupts(GLOBAL);
//Setup_Oscillator parameter not selected from Intr Oscillator Config tab

   // TODO: USER CODE!!

c1h;c2h;c3h;
while(true)
{
   if(sonne==0)
   {
      if(!fenetre1 && alarme==1 || !fenetre2 && alarme==1 || !fenetre3 && alarme==1 || !fenetre4 && alarme==1)
      {
         seconde=0;
         sonne=1;
      }   
      if((!porte1 || !porte2) && alarme==1)
      {
         if(seconde<=10)
            alarme_off;   
         if(seconde>10)
         {
            seconde=0;
            sonne=1;
         }   
      }
      if(alarme==0)
         alarme_off;
   }
   if(sonne==1)
   {
      if(seconde<=10 && alarme==1)
         alarme_on;  
      if(seconde>10 || alarme==0)
      {
         alarme_off;
         alarme=0;
         sonne=0;
      }   
   }      
}
}
