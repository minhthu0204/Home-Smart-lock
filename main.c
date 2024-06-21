#include <main.h>
#define LCD_RS_PIN      PIN_D2
#define LCD_RW_PIN      PIN_D3
#define LCD_ENABLE_PIN  PIN_C5
#define LCD_DATA4       PIN_D4
#define LCD_DATA5       PIN_D5
#define LCD_DATA6       PIN_D6
#define LCD_DATA7       PIN_D7
#include <lcd.c>
//=============================KHAI BAO CHAN CHO MA TRAN PHIM==================
#define R1            PIN_B4
#define R2            PIN_B5
#define R3            PIN_B6
#define R4            PIN_B7
#define c1            PIN_B0
#define c2            PIN_B1
#define c3            PIN_B2
#define c4            PIN_B3
///============================KHAI BAO CHAN CHO RC522=========================  
#define MFRC522_CS         PIN_C4                 
#define MFRC522_SCK        PIN_C3
#define MFRC522_SI         PIN_C6                           
#define MFRC522_SO         PIN_C7             
#define MFRC522_RST        PIN_C2    
#include<Built_in.h>

//!//===========================Khai bao chuong trinh con======================

char enter_password[16]={'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};  // tao 1 mang de luu gia tri nhap sau do luu vao vdk\
char pass_worrd[16]={'1','2','3','#','0','0','0','0','0','0','0','0','0','0','0','0'};
void bipbip()
{
  output_high(PIN_A1);
  delay_ms(100);
  output_low(PIN_A1);   
}
void write_eeprom_pass()
{
   for(int i=0;i<16;i++)
   {
      write_eeprom(i,enter_password[i]);
   }
}

void initialize_password()
{
   for(int i=0;i<16;i++)
   {
      write_eeprom(i,pass_worrd[i]);
   }  
}
char QuetMaPhim()
{
   output_low(PIN_B0);
   output_high(PIN_B1);
   output_high(PIN_B2);
   output_high(PIN_B3);
   if(input(PIN_B4)==0)        return '1';
   else if(input(PIN_B5)==0)   return '2';
   else if(input(PIN_B6)==0)   return '3';
   else if(input(PIN_B7)==0)   return 'A';
   output_low(PIN_B1);
   output_high(PIN_B0);
   output_high(PIN_B2);
   output_high(PIN_B3);
   if(input(PIN_B4)==0)        return '4';
   else if(input(PIN_B5)==0)   return '5';
   else if(input(PIN_B6)==0)   return '6';
   else if(input(PIN_B7)==0)   return 'B';
   output_low(PIN_B2);
   output_high(PIN_B0);
   output_high(PIN_B1);
   output_high(PIN_B3);
   if(input(PIN_B4)==0)        return '7';
   else if(input(PIN_B5)==0)   return '8';
   else if(input(PIN_B6)==0)   return '9';
   else if(input(PIN_B7)==0)   return 'C';
   output_low(PIN_B3);
   output_high(PIN_B0);
   output_high(PIN_B1);
   output_high(PIN_B2);
   if(input(PIN_B4)==0)        return '*';
   else if(input(PIN_B5)==0)   return '0';
   else if(input(PIN_B6)==0)   return '#';
   else if(input(PIN_B7)==0)   return 'D';
   
   return 0xff;
}
char Key_phim()
{
   char key ;
   do
   {
      key = QuetMaPhim();   // doc gia tri phim
   }
   while(key == 0xFF);     // doi nguoi dung nhap phim
   while(QuetMaPhim() != 0xFF);     // doi nguoi dung nha phim nhan de tro ve 0xFF
   return key;
}
void enter_pass()
 {
   for(int i=0; i<16;i++)
   {
      enter_password[i]= Key_phim();  // doc gia tri cua phim
      bipbip();
      if(enter_password[i] =='#') 
      {
         break;
      }
      lcd_gotoxy(i+1,2);
      printf(lcd_putc,"%c",enter_password[i]);
   }
 }
void empty_password()       // lam rong pass cau hinh lai
{
      for(int i=0;i<16;i++)
   {
     enter_password[i] = '0';
   }
}
int sosanh()
{
   for(int i=0;i<16;i++)
   {
      if(read_eeprom(i) != enter_password[i]) return 0;   // kiem tra mat khau doc tung phan tu sosanh
   }
   return 1;
}
void DongcoMocua()
{
      for( int i=0; i<50;i++)
      {
         output_high(PIN_E2);
         delay_us(2000);
         output_low(PIN_E2);
         delay_us(18000);
      }
}
void DongcoDongCua()
{
      for( int i=0; i<50;i++)
      {
         output_high(PIN_E2);
         delay_us(1000);
         output_low(PIN_E2);
         delay_us(19000);
      }
}
void Mocua()
{
   lcd_gotoxy(2,1);
   lcd_putc("\f    MOI VAO");
   lcd_gotoxy(2,2);
   lcd_putc("\f    CUA MO");
   output_high(PIN_A0);
   DongcoMocua();
}
 //// chuong trinh quet the tag
char DATA_PHU[4]    ={0XD3, 0X85, 0XCB, 0X1B};
char DATA_HAI[4]    ={0xB1, 0x96, 0xE0, 0x20};
int1 THE_1=0, TAM = 0, i;
char tt_1, tt_2;

int1 QUET_THE(char DATA[],char UID[])
{
   FOR (int i = 0; i < 4; i++)
   {
      if(UID[i]== DATA[i] )
      {
         THE_1=1;
      }
      else
      {
         THE_1=0;
         break;
      }
   }
   return THE_1;
}

void WARRING() 
{ 
   for(int i=0; i<4;i++)
   {
      output_high(pin_A1);
      delay_ms(100);
      output_low(pin_A1);
   }   
}
void main()
{
   set_tris_d(0);
   set_tris_c(0);
   set_tris_b(0xF0);
   set_tris_a(0);
   set_tris_e(0x03);
   port_b_pullups(0xf0);
   output_low(PIN_A0);
   output_low(PIN_A1);
   output_low(PIN_E2);
   CHAR UID[6];
   UNSIGNED int TagType;
   lcd_init();
   lcd_gotoxy(1,1);
   printf(lcd_putc, "HE THONG MO CUA");
   lcd_gotoxy(6,2);
   printf(lcd_putc, "NHOM 6");
   delay_ms(1000);
   printf (lcd_putc, "\f  Loading...."); 
   MFRC522_Init();
   delay_ms(100);
   printf (lcd_putc, "\n*****Done!******"); 
   delay_ms(1000);
   while(True)
   {
HOME:
        printf (LCD_PUTC,"\fXin moi quet the"); 
        if(MFRC522_isCard (&TagType)) //Check any card
        {                                           
         //Read ID 
         IF (MFRC522_ReadCardSerial (&UID))             
         {
            tt_1 = QUET_THE(DATA_PHU,UID);
            tt_2 = QUET_THE(DATA_HAI,UID);   
            if( tt_1 == 1)
            { 
               if(TAM == 0)
               {
                  printf(LCD_PUTC, "\f Vo Tan Phu ");
                  lcd_gotoxy(1,2);
                  printf(LCD_PUTC, "xin moi ban vao");
                  bipbip();
                  delay_ms(100);
                  output_High(PIN_A0);
                  DongcoMocua(); 
                  goto HOME;
               }
            }
            else if( tt_2 == 1)
            { 
               if(TAM == 0)
               {
                  printf(LCD_PUTC, "\f TO DUY HAI ");
                  lcd_gotoxy(1,2);
                  printf(LCD_PUTC, "xin moi ban vao");
                  bipbip();
                  delay_ms(100);
                  output_High(PIN_A0);
                  DongcoMocua(); 
                  goto HOME;
               }
               TAM =~ TAM;
               delay_ms(1000);
            }
            else
            {
               lcd_gotoxy(1, 1);
               printf (LCD_PUTC, "The khong hop le");
               lcd_gotoxy(4, 2);
               printf (LCD_PUTC, "WARNING!!!");
               WARRING();
            } 
         }                                      
         
        MFRC522_Halt () ;    
      }
      else if(input(PIN_E1)==0)
      {
      lcd_gotoxy(1,2);                  
      lcd_putc("\fCua da duoc dong");               
      delay_ms(100);
      output_low(PIN_A0);
      DongcoDongCua();
           if(input(PIN_E0)==0)
           {
            while(input(PIN_E0)==0);
            lcd_gotoxy(1,1);
            lcd_putc("\f  * Input key");
            lcd_gotoxy(1,2);
            lcd_putc(" # Replace");
      while(1)
  {
   char check = Key_phim();// kiem tra xem nguoi dung nhan phim nao # hay *
   if(check == '*')
      {
AGAIN:
         lcd_putc("\f");
         lcd_gotoxy(1,1);
         printf(lcd_putc,"Enter pass:");
         enter_pass();   // doc gia tri nhap vao sau do hien thi len man hinh
         if(sosanh()==1)
         {
            lcd_putc("\f MAT KHAU DUNG");
            empty_password();
            delay_ms(2000);
            lcd_putc("\f");
            Mocua();
            goto HOME;
         }
         else if(sosanh()==0)
         {
            lcd_putc("\f MAT KHAU SAI");
            empty_password();
            delay_ms(2000);
            goto AGAIN;
         }
      }
      else if(check=='A')
      {
         empty_password();
         goto HOME;
      }
      else if(check == 'D')
      {
         empty_password();
         initialize_password();
      }
      else if(check == '#')
      {
         lcd_putc("\f");
         lcd_gotoxy(1,1);
         printf(lcd_putc,"Enter pass old:");
         enter_pass();  // doc gia tri
         if(sosanh()==1)
         {
            lcd_putc("\f");
            lcd_gotoxy(1,1);
            printf(lcd_putc,"YES NEW PASS:");
            empty_password();
            enter_pass();
            write_eeprom_pass();
            empty_password();
            lcd_putc("\f");
            lcd_gotoxy(1,1);
            printf(lcd_putc,"PASS DA SAVE");   
            delay_ms(2000);
         }
         else if(sosanh()==0)
         {
            lcd_putc("\f");
            lcd_gotoxy(1,1);
            printf(lcd_putc,"KHONG HOP LE !");
            empty_password();
            delay_ms(2000);
         }        
      }
     }
    }
   }
   }
}
