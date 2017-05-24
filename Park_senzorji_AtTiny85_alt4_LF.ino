// najprej številke pinov na ATTINY85-PU

#define LEDrd 0
#define LEDor 1
#define TIPKA 2
#define IZHOD 4
#define RVHOD 3

//izhodi
boolean ledRD=1;
boolean ledOR=0;
boolean izhod=0;
boolean pizhod=0;
boolean timer=0;
boolean ptimer=0;
boolean pris=0;
boolean ppris=0;
//boolean vklopBL=0;

//izhodi vmesni
boolean ri=0;
boolean ti=0;
//boolean utripajLED=0;
//boolean vrLED=0;

//vhodi
byte t=0;
byte r=0;

//prejšnja stanja
byte pt=0;
byte pr=0;
byte pti=0;
byte pri=0;
  
//stejemo cas
int iR=0;
int iT=0;
int iP=0;

//drugo

unsigned long stevec=0;
//int stevecBlink=0;

//za utripanje v prisilni OFF poziciji

void setup()
{
  pinMode(LEDrd, OUTPUT);
  pinMode(LEDor, OUTPUT);
  pinMode(TIPKA, INPUT_PULLUP);
  pinMode(IZHOD, OUTPUT);
  pinMode(RVHOD, INPUT_PULLUP);
  
  digitalWrite(LEDrd, HIGH);
  digitalWrite(LEDor, LOW);
  digitalWrite(IZHOD, LOW);
  
}

void loop()
{
  //preverimo in zapišemo stanje vhodov
  if(digitalRead(TIPKA)==HIGH) t=0;
  else if(digitalRead(TIPKA)==LOW) t=1;
  
  if(digitalRead(RVHOD)==HIGH) r=0;
  else if(digitalRead(RVHOD)==LOW) r=1;
  
  pizhod = digitalRead(IZHOD);
  //-----------------------------------
  
  if(pr==0 && r==0){
    ri=0;
  }
  else if(pr==0 && r==1){
    ri=0;
    iR=1;
  }
  else if(pr==1 && r==1){
    if(iR>0 && iR<5000){
      iR++;
    }
    else if(iR>=5000){
      ri=1;
      iR=1;
    }
  }
  else if(pr==1 && r==0){
    ri=0;
    iR=0;
  }
  
 
  
  if(pt==0 && t==0){
    ti=0;
  }
  else if(pt==0 && t==1){
    ti=0;
    iT=1;
  }
  else if(pt==1 && t==1){
    if(iT>0 && iT<5000){
      iT++;  
    }
    else if(iT>=5000){
      ti=1;
      iT=1;
    }
  }
  else if(pt==1 && t==0){
    ti=0;
    iT=0;
  }
  
//##############izračun iz Logic Friday####################  

izhod = ~ppris&(~ptimer|ptimer)&(~ri&~pri|ri&pri)&(pti&pizhod&(~ti|ti)|ti&~pti&~pizhod)|~pti&(~ppris&~ti&pizhod&(~pri|pri)&(~ptimer&(~ri|ri)|ri&ptimer)|ri&~pizhod&(~ppris&~ti&~pri&ptimer|~ptimer&(~ppris&~ti&~pri|ppris&ti&pri)));
timer = ~ppris&(~pti&~ptimer&(ti&~ri&~pri&~pizhod|~ti&pizhod&(~pri&(~ri|ri)|~ri&pri))|pti&(ti&ri&pri&ptimer&(~pizhod|pizhod)|~ri&~pri&(ptimer&(~pizhod|pizhod)|pizhod&~ptimer)&(~ti|ti)));
pris = ~ppris&ti&~pti&ri&pri&pizhod&~ptimer|ppris&(~ti&~pti&~pizhod&~ptimer&(~ri&~pri|ri&pri)|(pti&~pizhod&~ptimer&(~ri&~pri|ri&pri)|(~pizhod&~ptimer&(ri&~pri|~ri&pri)|(ptimer&(~pizhod|pizhod)|pizhod&~ptimer)&(~pri|pri)&(~ri|ri))&(~pti|pti))&(~ti|ti));

//##############izračun iz Logic Friday####################  
  ptimer=0;
  
  zapisiIzhode(izhod);
  
  if(timer==1){
    stevec+=1; 
    ptimer=0;
    if(stevec==550000){ //150.000 je za ca 12 sekund (12500 na sek), za 1,5 min bi rabil potem 1.125.000
      stevec=0;
      ptimer=1;
    }
  }
  else{
    stevec=0;
  }
  
//////////////////////////Utripanje LED pri prisilki

if(ppris==1 && pris==1){
  if(iP>=0 && iP<2500){
    iP++;
  }
  else if(iP>=2500 && iP<=3000){
    iP++;
    digitalWrite(LEDor, 1);
  }
  else if(iP>3000){
    digitalWrite(LEDor, 0);
    iP=0;
  }
}
else if(ppris==0 && pris==0){
  iP=0;
}

//////////////////////////Utripanje LED pri prisilki
  
// zapiši prejšnje vrednosti
  pt=t;
  pr=r;
  pti=ti;
  pri=ri;
  
  ppris=pris;

}


void zapisiIzhode(boolean izh){
    digitalWrite(IZHOD, izh);
    digitalWrite(LEDrd, !izh);
    digitalWrite(LEDor, izh);

}

