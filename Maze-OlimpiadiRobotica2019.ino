
  /*============================================================
   * AUTHOR: FRANCESCO RINALDI
   * ============================================================
   * DESCRIPTION: Program used to solve  mazes 
   * ============================================================
  
    TODO: Quello di ieri
                                                  
    
    */


    #include <Servo.h>
    #include <NewPing.h>
    Servo leftWheel;                   
    Servo rightWheel;      
    #define pinLeftWheel            10   
    #define pinRightWheel           9   

    
    #define StopValue 90
    #define ForwardValueRight 0
    #define ForwardValueLeft 180
    #define BackValueRight 180   
    #define BackValueLeft 0   

    #define nodelay 0              
    #define delay_giro_180 1028
    //#define delay_giro_90 514  
    #define delay_giro_90 450
    #define delay_giro_45 257
    #define delay_giro_60 300
    #define delay_giro_30 150
                                                                                               
    #define TriggerDestro 6
    #define EchoDestro 5
    #define TriggerSinistro 8
    #define EchoSinistro 7
    #define TriggerFronte 11
    #define EchoFronte 12

    #define LimiteFronte 15
    

    float SinistraUltimaDistanza=0;
    float DestraUltimaDistanza=0;
    float FronteUltimaDistanza=0;

    float DestraPenultimaDistanza;
    float SinistraPenultimaDistanza;
    
    //Variable used to reduce the method's enter time
    float LimitePerCorreggereLaDirezione=1000;
    long TempoDallUltimoRaddrizzamentoVersoDestra=0;
    long TempoDallUltimoRaddrizzamentoVersoSinistra=0;

    long TempoUltimaCurva90=0;

   
    void stopWheels() {
      leftWheel.write(StopValue);
      rightWheel.write(StopValue);
    }
    
    void goForward()
    {
      rightWheel.write(ForwardValueRight);
        leftWheel.write(ForwardValueLeft);
    }
    


    void goForwards_() 
    {
      //Check left/right distances to correct its trajectory
     if ( FronteUltimaDistanza>20 && (DestraUltimaDistanza<=5 ) && ((millis()-TempoDallUltimoRaddrizzamentoVersoDestra)>LimitePerCorreggereLaDirezione) ) 
      {
        
        goLeft(50);
        rightWheel.write(ForwardValueRight);
        leftWheel.write(ForwardValueLeft);  
        delay(20);
       TempoDallUltimoRaddrizzamentoVersoDestra=millis();
      }
      else if( FronteUltimaDistanza>20 && (SinistraUltimaDistanza<=5 ) && ((millis()-TempoDallUltimoRaddrizzamentoVersoSinistra)>LimitePerCorreggereLaDirezione) )
      {
          goRight(50);
          rightWheel.write(ForwardValueRight);
          leftWheel.write(ForwardValueLeft);
          delay(20);
          TempoDallUltimoRaddrizzamentoVersoSinistra=millis();
      }
      else 
      {
        rightWheel.write(ForwardValueRight);
        leftWheel.write(ForwardValueLeft);
      } 
      
    }

    void goRight(int Rotation)
    {
        leftWheel.write(ForwardValueLeft);
        rightWheel.write(StopValue);
        delay(Rotation);
    }
    void goLeft(int Rotation)
    {
      leftWheel.write(StopValue);
      rightWheel.write(ForwardValueRight);
      delay(Rotation);
    }
  
    NewPing* Sinistra;
    NewPing* Destra;
    NewPing* Fronte;
    
    void setup() 
    {
         
      //Motor association
      leftWheel.attach(pinLeftWheel);
      rightWheel.attach(pinRightWheel);

      //Sensor association
      Destra= new NewPing(TriggerDestro, EchoDestro);
      Sinistra= new NewPing(TriggerSinistro, EchoSinistro);
      Fronte= new NewPing(TriggerFronte, EchoFronte);
      Serial.begin(9600);
      
    }
        
    void loop() 
    { 
     //read distnaces 
      FronteUltimaDistanza= Fronte->ping_cm();
      SinistraUltimaDistanza=Sinistra -> ping_cm();
      DestraUltimaDistanza= Destra -> ping_cm();


        //If there is space on front i am able to go forward
        if
        (FronteUltimaDistanza > DestraUltimaDistanza && FronteUltimaDistanza > SinistraUltimaDistanza)
       {
           goForwards_();
            delay(100);
       } 
      //While i am going forward there is an obstacle on front of me because the spece relived from the frontal US is less than "LimiteFronte"
      
      if(FronteUltimaDistanza<15)
      {
        //If there is more space on  Left go left with a 90 degrees Rotation
        if(SinistraUltimaDistanza>DestraUltimaDistanza+8)
              {
                 goLeft(delay_giro_90);
                 goForwards_();
                
                delay(250);
                TempoUltimaCurva90=millis();
                
              }     
         //If there is more space on  Right go Right with a 90 degrees Rotation     
        if(DestraUltimaDistanza >SinistraUltimaDistanza+8) 
          {
              goRight(delay_giro_90);
              goForwards_();
              delay(250);
              TempoUltimaCurva90=millis();
          }
         // It's possible that there is more space on Left or on right just because the robot is twisted
         // so if it's twested on right go little bit left
        if((DestraUltimaDistanza<=5 ) && (millis()-TempoDallUltimoRaddrizzamentoVersoSinistra)>LimitePerCorreggereLaDirezione ) 
         {
           goLeft(50);
           goForwards_();
           delay(20);
           TempoDallUltimoRaddrizzamentoVersoSinistra=millis();
        }    
        // so if it's twested on left go little bit right
        if((SinistraUltimaDistanza<=5 ) && (millis()-TempoDallUltimoRaddrizzamentoVersoDestra)>LimitePerCorreggereLaDirezione) 
        {
          goRight(50);
          goForwards_();
          delay(20);
          TempoDallUltimoRaddrizzamentoVersoDestra=millis();
        }   
         
        
      }  
      // while the robot is going forward the space on front is more than "LimiteFronte" and the left one is more than right one and front one so it's to turn left
      else if(FronteUltimaDistanza>30 && SinistraUltimaDistanza>FronteUltimaDistanza+25 && (millis()-TempoUltimaCurva90)>LimitePerCorreggereLaDirezione)
      {
          goLeft(delay_giro_45);
          goForwards_();
          delay(50);
      }
      // while the robot is going forward the space on front is more than "LimiteFronte" and the right one is more than left one and frontone so it's to turn left
      else if (FronteUltimaDistanza>30 && DestraUltimaDistanza>FronteUltimaDistanza+25 && (millis()-TempoUltimaCurva90)>LimitePerCorreggereLaDirezione)
      {
        goRight(delay_giro_45);
        goForwards_();
        delay(50);
      }

    
           
    }


    
