 // For RAMPS 1.4
#define X_STEP_PIN        54
#define X_DIR_PIN         55
#define X_ENABLE_PIN      38
#define X_MIN_PIN         3
#define X_MAX_PIN         2

#define Y_STEP_PIN        60
#define Y_DIR_PIN         61
#define Y_ENABLE_PIN      56
#define Y_MIN_PIN         14
#define Y_MAX_PIN         15

#define Z_STEP_PIN        46
#define Z_DIR_PIN         48
#define Z_ENABLE_PIN      62
#define Z_DER_PIN         18
#define Z_IZQ_PIN         19

#define X_FRONT_PIN       32
#define X_BEHIND_PIN      47
#define Y_FRONT_PIN       45
#define Y_BEHIND_PIN      43
#define Z_FRONT_PIN       41
#define Z_BEHIND_PIN      39

#define PRINT_POINT       31  
#define SAVE_POINT_1      33
#define SAVE_POINT_2      35

#define START_HALL_SENSOR 23
#define END_HALL_SENSOR   25
#define END               27
char arranque[200];
char xyz_points[42];
int V_measure = 30;
int V_manual = 100;
int tiempoDelay_s1 = 500;    // Tiempo de espera para que la sonda deje de vibrar
int tiempoDelay_s2 = 1000;
char AUX[10];
int STAR;
int MODE;
long int PX1;
long int PY1;
long int PZ1;
long int PX2;
long int PY2;
long int PZ2;
//   unsigned byte H0[2] = {0,0};
//   unsigned byte H[2] = {1,2};
long int Point1[3],Point2[3];
long int x,y,z;
long int Dx,Dy,Dz;
int       R_factor;
long int res_x = 16;                    // 160 steps by 1mm
long int res_yz = 80;                  // 801 steps by 1mm
long int resolution_x;                    // 160 steps by 1mm
long int resolution_yz;                  // 801 steps by 1mm
boolean sentido1 = LOW;                   // DERECHA
boolean sentido2 = HIGH;                 // IZQUIERDA
void X_Motor_Home();
void Y_Motor_Home();
void Z_Motor_Home();
void X_Move_1_STEP();
void Y_Move_1_STEP();
void Z_Move_1_STEP();
void X_1_STEP(boolean sentido);
void Y_1_STEP(boolean sentido);
void Z_1_STEP(boolean sentido);

void Parametric_Curve();
void Make_Matriz_xz();
void Make_Matriz_xy();
void Make_Matriz_yz();
void Make_Matriz_y();
void Data_Point();
void Print_Direction();
void setup() {

 Serial.begin(115200);
  Serial.println(".............. BIENVENIDOS .................");
  Serial.println("............................................");
  Serial.println(".. Envia '155@' en la terminal para iniciar  ..");
  Serial.println(".. Envia '1' en la terminal para cancelar ..");
  Serial.println("............................................");

/* X Bridge PIN */
 pinMode(X_STEP_PIN,OUTPUT);
 pinMode(X_DIR_PIN,OUTPUT);
 pinMode(X_ENABLE_PIN,OUTPUT);
 pinMode(X_MIN_PIN,INPUT);
 pinMode(X_MAX_PIN,INPUT);

/* Y Bridge PIN */
 pinMode(Y_STEP_PIN,OUTPUT);
 pinMode(Y_DIR_PIN,OUTPUT);
 pinMode(Y_ENABLE_PIN,OUTPUT);
 pinMode(Y_MIN_PIN,INPUT);
 pinMode(Y_MAX_PIN,INPUT);
/* Z Bridge PIN */
 pinMode(Z_STEP_PIN,OUTPUT);
 pinMode(Z_DIR_PIN,OUTPUT);
 pinMode(Z_ENABLE_PIN,OUTPUT);
 pinMode(Z_DER_PIN,INPUT);
 pinMode(Z_IZQ_PIN,INPUT);

pinMode(X_FRONT_PIN,INPUT);
pinMode(X_BEHIND_PIN,INPUT);
pinMode(Y_FRONT_PIN,INPUT);
pinMode(Y_BEHIND_PIN,INPUT);
pinMode(Z_FRONT_PIN,INPUT);
pinMode(Z_BEHIND_PIN,INPUT);

 pinMode(SAVE_POINT_1,INPUT);
 pinMode(SAVE_POINT_2,INPUT);
 pinMode(PRINT_POINT,INPUT);

 pinMode(START_HALL_SENSOR,OUTPUT);
 pinMode(END_HALL_SENSOR,INPUT);
  digitalWrite(START_HALL_SENSOR,LOW);
 
}

void loop() {
  
    STAR = 0;
    MODE = 0;
    PX1 = 0;
    PY1 = 0;
    PZ1 = 0;
    PX2 = 0;
    PY2 = 0;
    PZ2 = 0;
    if(arranque[0] != '\0'){
      
 Serial.begin(115200);
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            for (int g = 0;g<200;g++){arranque[g] = '\0';}
    }
  digitalWrite(START_HALL_SENSOR,LOW);
  digitalWrite(END,LOW);
   /* Enable Motors OUTPUTS Driver */
   digitalWrite(X_ENABLE_PIN,HIGH);
   digitalWrite(Y_ENABLE_PIN,HIGH);
   digitalWrite(Z_ENABLE_PIN,HIGH);
    if(Serial.available()>0)
    {Serial.readBytesUntil('*',arranque,100);
    
    int len = sizeof(arranque);
    int h = 0;
    int ff = 0;
    for(int i = 0;i < len; i++){
      
      if(arranque[i]  == '@'){
        h = h + 1;
        
        switch(h){
          case 1:
            STAR    = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          case 2:
            MODE    = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
            case 3:
            PX1     = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          case 4:
            PY1     = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          case 5:
            PZ1     = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          case 6:
            PX2     = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          case 7:
            PY2     = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          case 8:
            PZ2     = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          case 9:
            R_factor = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          case 10:
            V_measure = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          case 11:
            V_manual     = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          default:
            break;
          }
        }
       else{
         AUX[ff] = arranque[i];
         ff = ff + 1;
       }
    }
    
    }
    
      if(STAR == 35){
        
        Y_Motor_Home();
        X_Motor_Home();
        Z_Motor_Home();
        x = 0; y = 0; z=0;
        Parametric_Curve();
  
        }
        
        
        
    if(STAR == 155)
    {
     
    resolution_x = res_x*R_factor;                    // 160 steps by 1mm
    resolution_yz = res_yz*R_factor; 
      
      
      Y_Motor_Home();
      X_Motor_Home();
      Z_Motor_Home();
      x = 0; y = 0; z=0;
      digitalWrite(X_ENABLE_PIN,HIGH);
      digitalWrite(Y_ENABLE_PIN,HIGH);
      digitalWrite(Z_ENABLE_PIN,HIGH);
      if(MODE == 1){
              Serial.println("Posicione el primer punto");
              while(1){
                 X_Move_1_STEP();
                 Y_Move_1_STEP();
                 Z_Move_1_STEP();
                 
                 Print_Direction();
                 if((digitalRead(SAVE_POINT_1)== HIGH) && (digitalRead(SAVE_POINT_2)== LOW) ){
                  Point1[0] = x;
                  Point1[1] = y;
                  Point1[2] = z;
                  Serial.print("PUNTO 1");
                  Serial.print("@X1 = ");
                  Serial.print(Point1[0]);
                  Serial.print("@Y1 = ");
                  Serial.print(Point1[1]);
                  Serial.print("@Z1 = ");
                  Serial.print(Point1[2]);
                  Serial.print("@*");
                  break;
                 }
                 
              }
              Serial.println("Posicione el segundo punto");
               while(1){
                 X_Move_1_STEP();
                 Y_Move_1_STEP();
                 Z_Move_1_STEP();
                 Print_Direction();
                 if((digitalRead(SAVE_POINT_1)== LOW) && (digitalRead(SAVE_POINT_2)== HIGH)){
                  Point2[0] = x;
                  Point2[1] = y;
                  Point2[2] = z;
                  Serial.print("PUNTO 2");
                  Serial.print("@X2 = ");
                  Serial.print(Point2[0]);
                  Serial.print("@Y2 = ");
                  Serial.print(Point2[1]);
                  Serial.print("@Z2 = ");
                  Serial.println(Point2[2]);
                  Serial.print("@*");
                  break;
                 }
              }
      }
      delay(1000);
      if(MODE == 2){
              Serial.println("Puntos dados por el usuario");
                 X_Move_1_STEP();
                 Y_Move_1_STEP();
                 Z_Move_1_STEP();
                 Print_Direction();
                  Point1[0] = PX1;
                  Point1[1] = PY1;
                  Point1[2] = PZ1;
                  Point2[0] = PX2;
                  Point2[1] = PY2;
                  Point2[2] = PZ2;     
                  Serial.print("@*");   

      }
      
      
      Y_Motor_Home();
      X_Motor_Home();
      Z_Motor_Home();
      x = 0; y = 0; z=0;

      Dx = Point2[0] - Point1[0];
      Dy = Point2[1] - Point1[1];
      Dz = Point2[2] - Point1[2];  
              Serial.println("Diferencias");
                  Serial.print(Dx);
                  Serial.print("  ");
                  Serial.print(Dy);
                  Serial.print("  ");
                  Serial.print(Dz);
                  Serial.print("@*");
      
      if((Dx > 0)&&(Point2[1] == Point1[1])&&(Dz > 0)){
        Serial.print("Matriz XZ");
        Make_Matriz_xz();
      }

       if((Dx > 0)&&(Dy > 0)&&(Point2[2] == Point1[2])){
        Serial.print("Matriz XY");
        Make_Matriz_xy();
      }

       if((Point2[0] == Point1[0])&&(Dy > 0)&&(Dz > 0)){
        Serial.print("Matriz YZ");
        Make_Matriz_yz();
      }

      if((Point2[0] == Point1[0])&&(Dy > 0)&&(Point2[2] == Point1[2])){
        Serial.print("Matriz Y");
        Make_Matriz_y();
      }
      
      if((Dx > 0)&&(Point2[1] == Point1[1])&&(Point2[2] == Point1[2])){
        Serial.print("Matriz X");
        Make_Matriz_x();
      }
      
      if((Point2[0] == Point1[0])&&(Point2[1] == Point1[1])&&(Dz > 0)){
        Serial.print("Matriz Z");
        Make_Matriz_z();
      }

      if((Point2[0] == Point1[0])&&(Point2[1] == Point1[1])&&(Point2[2] == Point1[2])){
        Serial.print("DATOS PUNTO");
        Data_Point();      
      }
    Serial.flush();
  }
  delay(1000);
  
}


/* Parametric Curve*/

void Parametric_Curve(){
  pinMode(END,INPUT);
  boolean sentidox;
  boolean sentidoy;
  boolean sentidoz;
  long int x0 = 0,y0 = 0,z0 = 0;
  int vx,vy,vz;

    
  while(1){
      
  if(xyz_points[0] != '\0'){
     for (int g = 0;g<7;g++){AUX[g] = '\0';}
     for (int g = 0;g<42;g++){xyz_points[g] = '\0';}
    }
    
             digitalWrite(X_ENABLE_PIN,HIGH);
             digitalWrite(Y_ENABLE_PIN,HIGH);
             digitalWrite(Z_ENABLE_PIN,HIGH);
    if(digitalRead(END) == HIGH){
    while(1){  
    if(Serial.available()>0)
    {Serial.readBytesUntil('*',xyz_points,100);
    
    int len_1 = sizeof(xyz_points);
    int h = 0;
    int ff = 0;
    for(int i = 0;i < len_1; i++){
      
      if(xyz_points[i]  == '@'){
        h = h + 1;
        
        switch(h){
          case 1:
            x    = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          case 2:
            y    = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
            case 3:
            z     = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          case 4:
            vx     = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          case 5:
            vy     = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          case 6:
            vz     = String(AUX).toInt();
            for (int g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          default:
            break;
          }
        }
       else{
         AUX[ff] = xyz_points[i];
         ff = ff + 1;
       }}
     
     
      Dx = x - x0;
      Dy = y - y0;
      Dz = z - z0; 
   // x sentido2 izquierda
   // y sentido1 abajo
   // z sentido2 atras
     if(Dx < 0){sentidox = sentido2;}else{sentidox = sentido1;}
     if(Dy < 0){sentidoy = sentido1;}else{sentidoy = sentido2;}
     if(Dz < 0){sentidoz = sentido2;}else{sentidoz = sentido1;}
   
   while(1){
          if(digitalRead(END_HALL_SENSOR) == HIGH){
              for(int p_x = 0;p_x<abs(Dx);p_x++){X_1_STEP(sentidox);}
              for(long int p_z = 0;p_z<abs(Dz);p_z++){Z_1_STEP(sentidoz);}
              for(long int p_y = 0;p_y<abs(Dy);p_y++){Y_1_STEP(sentidoy);}
              delay(tiempoDelay_s2);
              digitalWrite(START_HALL_SENSOR,HIGH);
              delay(10);
              digitalWrite(START_HALL_SENSOR,LOW);
            
          break;
            }}
          break;
        }
          
        }
 x0 = x;
 y0 = y;
 z0 = z;
 
 }
       
    else{break;}
  
  }}


/*  X axis Motor send to Home */
void X_Motor_Home(){
    if(digitalRead(X_MIN_PIN)==LOW)
    {
    digitalWrite(X_ENABLE_PIN,LOW);
    while(1)
    {
      digitalWrite(X_DIR_PIN,sentido2);  // izquierda
      digitalWrite(X_STEP_PIN,HIGH);
      delayMicroseconds(V_measure);
      digitalWrite(X_STEP_PIN,LOW);
      delayMicroseconds(V_measure);
      if(digitalRead(X_MIN_PIN)==HIGH)
      {break;}
    }}}

/*  Y axis Motor send to Home */
void Y_Motor_Home()
{
    if(digitalRead(Y_MIN_PIN)==LOW)
    {
    digitalWrite(Y_ENABLE_PIN,LOW);
    while(1)
    {
      digitalWrite(Y_DIR_PIN,sentido1); // abajo
      digitalWrite(Y_STEP_PIN,HIGH);
      delayMicroseconds(V_measure);
      digitalWrite(Y_STEP_PIN,LOW);
      delayMicroseconds(V_measure);
      if(digitalRead(Y_MIN_PIN)==HIGH)
      {break;}
    }}}

/*  Z axis Motor send to Home */
void Z_Motor_Home()
{
    if((digitalRead(Z_DER_PIN)==LOW)&&(digitalRead(Z_IZQ_PIN)==LOW))
    {
    digitalWrite(Z_ENABLE_PIN,LOW);
    while(1)
    {
      digitalWrite(Z_DIR_PIN,sentido2);  // atras
      digitalWrite(Z_STEP_PIN,HIGH);
      delayMicroseconds(V_measure);
      digitalWrite(Z_STEP_PIN,LOW);
      delayMicroseconds(V_measure);
      if((digitalRead(Z_DER_PIN)==HIGH)&&(digitalRead(Z_IZQ_PIN)==HIGH))
      {break;}
    }}}
    
/*  X axis Motor Move 1 Step */
void X_Move_1_STEP(){
  digitalWrite(X_ENABLE_PIN,HIGH);
  if((digitalRead(X_FRONT_PIN)==HIGH) && (digitalRead(X_BEHIND_PIN)==LOW)){
      digitalWrite(X_ENABLE_PIN,LOW);
      digitalWrite(X_DIR_PIN,sentido1);
      digitalWrite(X_STEP_PIN,HIGH);
      delayMicroseconds(3);
      digitalWrite(X_STEP_PIN,LOW);
      delayMicroseconds(V_manual);
      x++;
  }
  if((digitalRead(X_FRONT_PIN)==LOW) && (digitalRead(X_BEHIND_PIN)==HIGH))
    {
      if(digitalRead(X_MIN_PIN)==LOW){
        digitalWrite(X_ENABLE_PIN,LOW);
        digitalWrite(X_DIR_PIN,sentido2);
        digitalWrite(X_STEP_PIN,HIGH);
        delayMicroseconds(V_manual);
        digitalWrite(X_STEP_PIN,LOW);
        delayMicroseconds(V_manual);
        x--;
      }
      if(digitalRead(X_MIN_PIN)==HIGH){
        x = 0;
      }
    }    
}

/*  Y axis Motor Move 1 Step */
void Y_Move_1_STEP(){
  digitalWrite(Y_ENABLE_PIN,HIGH);
  if((digitalRead(Y_FRONT_PIN)==HIGH) && (digitalRead(Y_BEHIND_PIN)==LOW)){
      digitalWrite(Y_ENABLE_PIN,LOW);
      digitalWrite(Y_DIR_PIN,sentido2);
      digitalWrite(Y_STEP_PIN,HIGH);
      delayMicroseconds(V_manual);
      digitalWrite(Y_STEP_PIN,LOW);
      delayMicroseconds(V_manual);
      y++;
  }
  if((digitalRead(Y_FRONT_PIN)==LOW) && (digitalRead(Y_BEHIND_PIN)==HIGH))
    {
      if(digitalRead(Y_MIN_PIN)==LOW){
        digitalWrite(Y_ENABLE_PIN,LOW);
        digitalWrite(Y_DIR_PIN,sentido1);
        digitalWrite(Y_STEP_PIN,HIGH);
        delayMicroseconds(V_manual);
        digitalWrite(Y_STEP_PIN,LOW);
        delayMicroseconds(V_manual);
        y--;
      }
      if(digitalRead(Y_MIN_PIN)==HIGH){
        y = 0;
      }
    }    
}

/*  Z axis Motor Move 1 Step */
void Z_Move_1_STEP(){
  digitalWrite(Z_ENABLE_PIN,HIGH);
  if((digitalRead(Z_FRONT_PIN)==HIGH) && (digitalRead(Z_BEHIND_PIN)==LOW)){
      digitalWrite(Z_ENABLE_PIN,LOW);
      digitalWrite(Z_DIR_PIN,sentido1);
      digitalWrite(Z_STEP_PIN,HIGH);
      delayMicroseconds(V_manual);
      digitalWrite(Z_STEP_PIN,LOW);
      delayMicroseconds(V_manual);
      z++;
  }
  if((digitalRead(Z_FRONT_PIN)==LOW) && (digitalRead(Z_BEHIND_PIN)==HIGH))
    {
      if((digitalRead(Z_DER_PIN)==LOW)&&(digitalRead(Z_IZQ_PIN)==LOW)){
        digitalWrite(Z_ENABLE_PIN,LOW);
        digitalWrite(Z_DIR_PIN,sentido2);
        digitalWrite(Z_STEP_PIN,HIGH);
        delayMicroseconds(V_manual);
        digitalWrite(Z_STEP_PIN,LOW);
        delayMicroseconds(V_manual);
        z--;
      }
      if((digitalRead(Z_DER_PIN)==HIGH)&&(digitalRead(Z_IZQ_PIN)==HIGH)){
        z = 0;
      }
    }    
}
/*  Move X axis 1 Step */
void X_1_STEP(boolean sentido){
  digitalWrite(X_ENABLE_PIN,LOW);
  digitalWrite(X_DIR_PIN,sentido);
  digitalWrite(X_STEP_PIN,HIGH);
  delayMicroseconds(V_measure);
  digitalWrite(X_STEP_PIN,LOW);
  delayMicroseconds(V_measure);
  
}
/*  Move Y axis 1 Step */
void Y_1_STEP(boolean sentido){
  digitalWrite(Y_ENABLE_PIN,LOW);
  digitalWrite(Y_DIR_PIN,sentido);
  digitalWrite(Y_STEP_PIN,HIGH);
  delayMicroseconds(V_measure);
  digitalWrite(Y_STEP_PIN,LOW);
  delayMicroseconds(V_measure);
}
/*  Move Z axis 1 Step */
void Z_1_STEP(boolean sentido){
  digitalWrite(Z_ENABLE_PIN,LOW);
  digitalWrite(Z_DIR_PIN,sentido);
  digitalWrite(Z_STEP_PIN,HIGH);
  delayMicroseconds(V_measure);
  digitalWrite(Z_STEP_PIN,LOW);
  delayMicroseconds(V_measure);
}

/*  Make Matriz on x and z axis */
void Make_Matriz_xz()
{
  long int rows = Dz/resolution_yz;
  long int columns = Dx/resolution_x;
  Serial.print("@FILAS = ");
  Serial.print(rows);
  Serial.print("@COLUMNAS = ");
  Serial.print(columns);
  Serial.print("@+*");

    for(int i=0;i<rows;i++){
      if (i == 0){
        for(int p_x = 0;p_x<Point1[0];p_x++){// send X,Y,Z axis to Point1
          X_1_STEP(sentido1);
        }
        for(int p_z = 0;p_z<Point1[2];p_z++){
          Z_1_STEP(sentido1);
        }
        for(int p_y = 0;p_y<Point1[1];p_y++){
          Y_1_STEP(sentido2);
        }
      }
      for(int j=0;j<columns;j++){
        while(1){
             digitalWrite(X_ENABLE_PIN,HIGH);
             digitalWrite(Y_ENABLE_PIN,HIGH);
             digitalWrite(Z_ENABLE_PIN,HIGH);
          if(digitalRead(END_HALL_SENSOR) == HIGH){
            if(j == 0){
             digitalWrite(START_HALL_SENSOR,HIGH);
             delay(10);
             digitalWrite(START_HALL_SENSOR,LOW);
            }
            else{
              for(int p_x = 0;p_x<resolution_x;p_x++){
              X_1_STEP(sentido1);}
              delay(tiempoDelay_s1);
              digitalWrite(START_HALL_SENSOR,HIGH);
              delay(10);
              digitalWrite(START_HALL_SENSOR,LOW);
            }
            
          break;
            }}}
            while(1){
            if(digitalRead(END_HALL_SENSOR) == HIGH){
            break;}}
       for(long int p_x = 0;p_x<((columns-1)*resolution_x);p_x++){
              X_1_STEP(sentido2);}
        for(long int p_z = 0;p_z<resolution_yz;p_z++){
              Z_1_STEP(sentido1);}
              delay(tiempoDelay_s1);      
            }
        while(1){
        if(digitalRead(END_HALL_SENSOR) == HIGH){
        break;}}
        digitalWrite(END,HIGH);
        delay(10);
        digitalWrite(END,LOW);
  Y_Motor_Home();
  X_Motor_Home();
  Z_Motor_Home();
}

/*  Make Matriz on x and y axis */
void Make_Matriz_xy()
{
  long int rows = Dy/resolution_yz;
  long int columns = Dx/resolution_x;
  Serial.print("@FILAS = ");
  Serial.print(rows);
  Serial.print("@COLUMNAS = ");
  Serial.print(columns);
  Serial.print("@+*");

    for(int i=0;i<rows;i++){
      if (i == 0){
        for(int p_x = 0;p_x<Point1[0];p_x++){// send X,Y,Z axis to Point1
          X_1_STEP(sentido1);
        }
        for(int p_z = 0;p_z<Point1[2];p_z++){
          Z_1_STEP(sentido1);
        }
        for(int p_y = 0;p_y<Point1[1];p_y++){
          Y_1_STEP(sentido2);
        }
      }
      for(int j=0;j<columns;j++){
        while(1){
             digitalWrite(X_ENABLE_PIN,HIGH);
             digitalWrite(Y_ENABLE_PIN,HIGH);
             digitalWrite(Z_ENABLE_PIN,HIGH);
          if(digitalRead(END_HALL_SENSOR) == HIGH){
            if(j == 0){
             digitalWrite(START_HALL_SENSOR,HIGH);
             delay(10);
             digitalWrite(START_HALL_SENSOR,LOW);
            }
            else{
              for(int p_x = 0;p_x<resolution_x;p_x++){
              X_1_STEP(sentido1);}
              delay(tiempoDelay_s1);
              digitalWrite(START_HALL_SENSOR,HIGH);
              delay(10);
              digitalWrite(START_HALL_SENSOR,LOW);
            }
            
          break;
            }}}
            while(1){
            if(digitalRead(END_HALL_SENSOR) == HIGH){
            break;}}
       for(long int p_x = 0;p_x<((columns-1)*resolution_x);p_x++){
              X_1_STEP(sentido2);}
        for(long int p_y = 0;p_y<resolution_yz;p_y++){
              Y_1_STEP(sentido2);}
              delay(tiempoDelay_s1);      
            }
        while(1){
        if(digitalRead(END_HALL_SENSOR) == HIGH){
        break;}}
        digitalWrite(END,HIGH);
        delay(10);
        digitalWrite(END,LOW);
  Y_Motor_Home();
  X_Motor_Home();
  Z_Motor_Home();
}

/*  Make Matriz on y and z axis */
void Make_Matriz_yz()
{
 long int rows = Dz/resolution_yz;
 long int columns = Dy/resolution_yz;
  Serial.print("@FILAS = ");
  Serial.print(rows);
  Serial.print("@COLUMNAS = ");
  Serial.print(columns);
  Serial.print("@+*");

    for(int i=0;i<rows;i++){
      if (i == 0){
        for(int p_x = 0;p_x<Point1[0];p_x++){// send X,Y,Z axis to Point1
          X_1_STEP(sentido1);
        }
        for(int p_z = 0;p_z<Point1[2];p_z++){
          Z_1_STEP(sentido1);
        }
        for(int p_y = 0;p_y<Point1[1];p_y++){
          Y_1_STEP(sentido2);
        }
      }
      for(int j=0;j<columns;j++){
        while(1){
             digitalWrite(X_ENABLE_PIN,HIGH);
             digitalWrite(Y_ENABLE_PIN,HIGH);
             digitalWrite(Z_ENABLE_PIN,HIGH);
          if(digitalRead(END_HALL_SENSOR) == HIGH){
            if(j == 0){
             digitalWrite(START_HALL_SENSOR,HIGH);
             delay(10);
             digitalWrite(START_HALL_SENSOR,LOW);
            }
            else{
              for(int p_y = 0;p_y<resolution_yz;p_y++){
              Y_1_STEP(sentido2);}
              delay(tiempoDelay_s1);
              digitalWrite(START_HALL_SENSOR,HIGH);
              delay(10);
              digitalWrite(START_HALL_SENSOR,LOW);
            }
            
          break;
            }}}
            while(1){
            if(digitalRead(END_HALL_SENSOR) == HIGH){
            break;}}
       for(long int p_y = 0;p_y<((columns-1)*resolution_yz);p_y++){
        
              Y_1_STEP(sentido1);}
        for(long int p_z = 0;p_z<resolution_yz;p_z++){
              Z_1_STEP(sentido1);}
              delay(tiempoDelay_s1);      
            }
        while(1){
        if(digitalRead(END_HALL_SENSOR) == HIGH){
        break;}}
        digitalWrite(END,HIGH);
        delay(10);
        digitalWrite(END,LOW);
  Y_Motor_Home();
  X_Motor_Home();
  Z_Motor_Home();
}

/*  Make Matriz on y axis */
void Make_Matriz_y(){
  int columns = Dy/resolution_yz;
  Serial.print("@PUNTOS = ");
  Serial.println(columns);
  Serial.print("@+*");
 
        for(int p_x = 0;p_x<Point1[0];p_x++){// send X,Y,Z axis to Point1
          X_1_STEP(sentido1);
        }
        for(int p_z = 0;p_z<Point1[2];p_z++){
          Z_1_STEP(sentido1);
        }
        for(int p_y = 0;p_y<Point1[1];p_y++){
          Y_1_STEP(sentido2);
        }
      
      for(int j=0;j<columns;j++){
        while(1){
             digitalWrite(X_ENABLE_PIN,HIGH);
             digitalWrite(Y_ENABLE_PIN,HIGH);
             digitalWrite(Z_ENABLE_PIN,HIGH);
          if(digitalRead(END_HALL_SENSOR) == HIGH){
            if(j == 0){
             digitalWrite(START_HALL_SENSOR,HIGH);
             delay(10);
             digitalWrite(START_HALL_SENSOR,LOW);
            }
            else{
              for(int p_y = 0;p_y<resolution_yz;p_y++){
              Y_1_STEP(sentido2);}
              delay(tiempoDelay_s1);
              digitalWrite(START_HALL_SENSOR,HIGH);
              delay(10);
              digitalWrite(START_HALL_SENSOR,LOW);
            }
            
          break;
            }}}
            while(1){
            if(digitalRead(END_HALL_SENSOR) == HIGH){
            break;}}  
        digitalWrite(END,HIGH);
        delay(10);
        digitalWrite(END,LOW);    
             
            Y_Motor_Home();
            X_Motor_Home();
            Z_Motor_Home();
}

/*  Make Matriz on x axis */
void Make_Matriz_x(){
  int columns = Dx/resolution_x;
  Serial.print("@PUNTOS = ");
  Serial.println(columns);
  Serial.print("@+*");
 
        for(int p_x = 0;p_x<Point1[0];p_x++){// send X,Y,Z axis to Point1
          X_1_STEP(sentido1);
        }
        for(int p_z = 0;p_z<Point1[2];p_z++){
          Z_1_STEP(sentido1);
        }
        for(int p_y = 0;p_y<Point1[1];p_y++){
          Y_1_STEP(sentido2);
        }
      
      for(int j=0;j<columns;j++){
        while(1){
             digitalWrite(X_ENABLE_PIN,HIGH);
             digitalWrite(Y_ENABLE_PIN,HIGH);
             digitalWrite(Z_ENABLE_PIN,HIGH);
          if(digitalRead(END_HALL_SENSOR) == HIGH){
            if(j == 0){
             digitalWrite(START_HALL_SENSOR,HIGH);
             delay(10);
             digitalWrite(START_HALL_SENSOR,LOW);
            }
            else{
              for(int p_x = 0;p_x<resolution_x;p_x++){
              X_1_STEP(sentido1);}
              delay(tiempoDelay_s1);
              digitalWrite(START_HALL_SENSOR,HIGH);
              delay(10);
              digitalWrite(START_HALL_SENSOR,LOW);
            }
            
          break;
            }}}
            while(1){
            if(digitalRead(END_HALL_SENSOR) == HIGH){
            break;}}  
        digitalWrite(END,HIGH);
        delay(10);
        digitalWrite(END,LOW);           
            Y_Motor_Home();
            X_Motor_Home();
            Z_Motor_Home();
}

/*  Make Matriz on z axis */
void Make_Matriz_z(){
  long int columns = Dz/resolution_yz;
  Serial.print("@PUNTOS = ");
  Serial.println(columns);
  Serial.print("@+*");
 
        for(int p_x = 0;p_x<Point1[0];p_x++){// send X,Y,Z axis to Point1
          X_1_STEP(sentido1);
        }
        for(int p_z = 0;p_z<Point1[2];p_z++){
          Z_1_STEP(sentido1);
        }
        for(int p_y = 0;p_y<Point1[1];p_y++){
          Y_1_STEP(sentido2);
        }
      
      for(int j=0;j<columns;j++){
        while(1){
             digitalWrite(X_ENABLE_PIN,HIGH);
             digitalWrite(Y_ENABLE_PIN,HIGH);
             digitalWrite(Z_ENABLE_PIN,HIGH);
          if(digitalRead(END_HALL_SENSOR) == HIGH){
            if(j == 0){
             digitalWrite(START_HALL_SENSOR,HIGH);
             delay(10);
             digitalWrite(START_HALL_SENSOR,LOW);
            }
            else{
              for(int p_z = 0;p_z<resolution_yz;p_z++){
              Z_1_STEP(sentido1);}
              delay(tiempoDelay_s1);
              digitalWrite(START_HALL_SENSOR,HIGH);
              delay(10);
              digitalWrite(START_HALL_SENSOR,LOW);
            }
            
          break;
            }}}
          while(1){
            if(digitalRead(END_HALL_SENSOR) == HIGH){
            break;}}  
        digitalWrite(END,HIGH);
        delay(10);
        digitalWrite(END,LOW);  
             
            Y_Motor_Home();
            X_Motor_Home();
            Z_Motor_Home();
}

///*  Take Data from 1 point*/
//void Data_Point(){
//
//  for(int p_x = 0;p_x<Point1[0];p_x++){// send X,Y,Z axis to Point1
//    X_1_STEP(sentido1);
//  }
//  for(int p_z = 0;p_z<Point1[2];p_z++){
//    Z_1_STEP(sentido1);
//  }
//  for(int p_y = 0;p_y<Point1[1];p_y++){
//    Y_1_STEP(sentido2);
//  }
//   for(int veces; veces<10;veces++){
//           
//      for(int i=0;i<10;i++){
//       digitalWrite(X_ENABLE_PIN,HIGH);
//       digitalWrite(Y_ENABLE_PIN,HIGH);
//       digitalWrite(Z_ENABLE_PIN,HIGH);
//        while(1){
//        if(digitalRead(END_HALL_SENSOR) == HIGH){
//        delay(tiempoDelay_s1);
//        digitalWrite(START_HALL_SENSOR,HIGH);
//        delay(10);
//        digitalWrite(START_HALL_SENSOR,LOW);
//        break;
//        }}}
//        while(1){
//          if(digitalRead(END_HALL_SENSOR) == HIGH){
//          break;}}
//       for(int j=0;j<(20*resolution_yz);j++){
//        Y_1_STEP(sentido1);
//       }
//        delay(1000);
//       for(int j=0;j<(20*resolution_yz);j++){
//        Y_1_STEP(sentido2);
//       }
//      }
//    Y_Motor_Home();
//    X_Motor_Home();
//    Z_Motor_Home();
//}

// Take Data from One point
void Data_Point(){

  for(int p_x = 0;p_x<Point1[0];p_x++){// send X,Y,Z axis to Point1
    X_1_STEP(sentido1);
  }
  for(int p_z = 0;p_z<Point1[2];p_z++){
    Z_1_STEP(sentido1);
  }
  for(int p_y = 0;p_y<Point1[1];p_y++){
    Y_1_STEP(sentido2);
  }
 
       digitalWrite(X_ENABLE_PIN,HIGH);
       digitalWrite(Y_ENABLE_PIN,HIGH);
       digitalWrite(Z_ENABLE_PIN,HIGH);
       Serial.println("Iniciar cronometro para 20 segundos");
        delay(20000); 
        while(1){ 
        if(digitalRead(END_HALL_SENSOR) == HIGH){
        delay(tiempoDelay_s1);
        digitalWrite(START_HALL_SENSOR,HIGH);
        delay(10);
        digitalWrite(START_HALL_SENSOR,LOW);
        break;
        }}
        while(1){
          if(digitalRead(END_HALL_SENSOR) == HIGH){
          break;}}
      
    Y_Motor_Home();
    X_Motor_Home();
    Z_Motor_Home();
}
/*  Print coordinades*/
void Print_Direction(){
  if(digitalRead(PRINT_POINT) == LOW){
    float xd = x;
    float yd = y;
    float zd = z;
    xd = xd/160;
    yd = yd/801;
    zd = zd/801;
    Serial.print("COORDENADA ACTUAL");
    Serial.print("@X = ");
    Serial.print(x);
    Serial.print("@Y = ");
    Serial.print(y);
    Serial.print("@Z = ");
    Serial.print(z);
    Serial.print("@DX = ");
    Serial.print(xd);
    Serial.print("@DY = ");
    Serial.print(yd);
    Serial.print("@DZ = ");
    Serial.print(zd);
    Serial.print("@*");
    delay(1000);
  }
}


