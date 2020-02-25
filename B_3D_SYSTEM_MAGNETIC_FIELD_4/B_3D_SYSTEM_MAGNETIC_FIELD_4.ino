 // For RAMPS 1.4
 
//#include "My_Funtions.h"
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

boolean sentido1 = LOW, sentido2 = HIGH;                   // DERECHA // IZQUIERDA
char arranque[100], xyz_points[100], AUX[7];
int V_measure = 30, V_manual = 10;
int tiempoDelay_s1 = 500, tiempoDelay_s2 = 10;    // Tiempo de espera para que la sonda deje de vibrar
int STAR, MODE, R_factor;
long int PX1, PY1, PZ1, PX2, PY2, PZ2, x = 0, y = 0, z = 0, Dx = 0, Dy = 0, Dz = 0;
long int Point1[3],Point2[3];
long int res_x = 16, res_yz = 20, resolution_x, resolution_yz;                    // 160 steps by 1mm // 801 steps by 1mm // 160 steps by 1mm
void X_Motor_Home(), Y_Motor_Home(), Z_Motor_Home(), Motors_Home(), X_Move_1_STEP(), Y_Move_1_STEP(), Z_Move_1_STEP();
void X_1_STEP(boolean sentido),Y_1_STEP(boolean sentido), Z_1_STEP(boolean sentido);

void READ_CONF_WORD(), CHOOSE_MATRIZ();

void Parametric_Curve();
void Make_Matriz_xz();
void Make_Matriz_xy();
void Make_Matriz_yz();
void Make_Matriz_y();
void Data_Point();
void Print_Direction();
void Configuration();
void XYZ_ENABLE(boolean mode);

void setup() {Configuration();}

void loop() {
  STAR = 0; MODE = 0; R_factor = 0; PX1 = 0; PY1 = 0; PZ1 = 0; PX2 = 0; PY2 = 0; PZ2 = 0;
  V_measure = 30, V_manual = 10;
  x = 0, y = 0, z = 0, Dx = 0, Dy = 0, Dz = 0;
  tiempoDelay_s1 = 500, tiempoDelay_s2 = 10;
  sentido1 = LOW, sentido2 = HIGH; 
  
  digitalWrite(START_HALL_SENSOR,LOW);
  digitalWrite(END,LOW);
  XYZ_ENABLE(HIGH);
  
  
  if(arranque[0] != '\0'){
    Configuration();
    for (int g = 0;g<7;g++){AUX[g] = '\0';}
    for (int g = 0;g<200;g++){arranque[g] = '\0';}}
   
  READ_CONF_WORD();   // Read Configuration Word    
   
  if(STAR == 35){
     Motors_Home();
     Serial.println("Home*");
     x = 0; y = 0; z=0;
     Parametric_Curve();
     Motors_Home();
     Serial.flush();}
        
  if(STAR == 155){
    
    resolution_x = res_x*R_factor;                    // 160 steps by 1mm
    resolution_yz = res_yz*R_factor; 
    Motors_Home();
    x = 0; y = 0; z=0;
    XYZ_ENABLE(HIGH);
    if(MODE == 1){
      Serial.println("Posicione el primer punto");
      while(1){
      X_Move_1_STEP();
      Y_Move_1_STEP();
      Z_Move_1_STEP();
      Print_Direction();
      if((digitalRead(SAVE_POINT_1)== HIGH) && (digitalRead(SAVE_POINT_2)== LOW) ){
        Point1[0] = x; Point1[1] = y; Point1[2] = z;
        Serial.print("PUNTO 1");
        Serial.print("@X1 = ");
        Serial.print(Point1[0]);
        Serial.print("@Y1 = ");
        Serial.print(Point1[1]);
        Serial.print("@Z1 = ");
        Serial.print(Point1[2]);
        Serial.print("@*");
        break;}}
        Serial.println("Posicione el segundo punto");
        while(1){
          X_Move_1_STEP(); Y_Move_1_STEP(); Z_Move_1_STEP();
          Print_Direction();
          if((digitalRead(SAVE_POINT_1)== LOW) && (digitalRead(SAVE_POINT_2)== HIGH)){
            Point2[0] = x; Point2[1] = y; Point2[2] = z;
            Serial.print("PUNTO 2");
            Serial.print("@X2 = ");
            Serial.print(Point2[0]);
            Serial.print("@Y2 = ");
            Serial.print(Point2[1]);
            Serial.print("@Z2 = ");
            Serial.println(Point2[2]);
            Serial.print("@*");
            break;}}}
      delay(1000);
    if(MODE == 2){
      Serial.println("Puntos dados por el usuario");
      X_Move_1_STEP(); Y_Move_1_STEP(); Z_Move_1_STEP();
      Print_Direction();
      Point1[0] = PX1; Point1[1] = PY1; Point1[2] = PZ1; Point2[0] = PX2; Point2[1] = PY2; Point2[2] = PZ2;     
      Serial.print("@*");}
    Motors_Home();
    x = 0; y = 0; z=0;
    Dx = Point2[0] - Point1[0]; Dy = Point2[1] - Point1[1]; Dz = Point2[2] - Point1[2];  
    Serial.println("Diferencias");
    Serial.print(Dx);
    Serial.print("  ");
    Serial.print(Dy);
    Serial.print("  ");
    Serial.print(Dz);
    Serial.print("@*");
    CHOOSE_MATRIZ(); 
    Serial.flush();
    Serial.end();
  }
  //delay(1000);
  
}


/* Parametric Curve*/

void Parametric_Curve(){
  //delay(1000);
  boolean wdog = true;
  pinMode(END,INPUT);
  boolean sentidox = HIGH;
  boolean sentidoy = LOW;
  boolean sentidoz = HIGH;
  long int x0 = 0,y0 = 0,z0 = 0;
  int vx = 0, vy = 0, vz = 0, g = 0, len_1 = 100, h = 0, ff = 0 , i = 0;
  long int Md = 0;
  XYZ_ENABLE(HIGH);
  AUX[0] = '\0'; xyz_points[0] = '\0';
    
  while (wdog){
    if (digitalRead(END) == LOW) {wdog = false; break;}
      AUX[0] = '\0'; xyz_points[0] = '\0';
      if (xyz_points[0] != '\0'){
        for (g = 0; g<7 ; g++) AUX[g] = '\0';
        for (g = 0; g<42; g++) xyz_points[g] = '\0';}
    
    if (digitalRead(END) == HIGH){
      while(wdog){  
        if (digitalRead(END) == LOW)  {wdog = false; break;}
        if (Serial.available()>0) {
        len_1 = Serial.readBytesUntil('*', xyz_points, 100);
        h = 0, ff = 0;
        Md = 0; Dx = 0; Dy = 0; Dz = 0;
    
        for (i = 0 ; i < len_1 ; i++){ 
          if (xyz_points[i]  == '@'){
            h = h + 1;
            switch(h){
              case 1:
                x    = String(AUX).toInt();
                for (g = 0;g<7;g++){AUX[g] = '\0';}
                ff = 0;
                break;
              case 2:
                y    = String(AUX).toInt();
                for (g = 0;g<7;g++){AUX[g] = '\0';}
                ff = 0;
                break;
              case 3:
                z     = String(AUX).toInt();
                for (g = 0;g<7;g++){AUX[g] = '\0';}
                ff = 0;
                break;
              case 4:
                vx     = String(AUX).toInt();
                for (g = 0;g<7;g++){AUX[g] = '\0';}
                ff = 0;
                break;
              case 5:
                vy     = String(AUX).toInt();
                for (g = 0;g<7;g++){AUX[g] = '\0';}
                ff = 0;
                break;
              case 6:
                vz     = String(AUX).toInt();
                for (g = 0;g<7;g++){AUX[g] = '\0';}
                ff = 0;
                break;
              default:
                ff = 0;
                break;}}
          else
            AUX[ff++] = xyz_points[i];}
     
 
        if (digitalRead(END) == LOW)  {wdog = false; break;}
     
        Dx = x - x0; Dy = y - y0; Dz = z - z0;
        if(Dx < 0){sentidox = sentido2;}else{sentidox = sentido1;}
        if(Dy < 0){sentidoy = sentido1;}else{sentidoy = sentido2;}
        if(Dz < 0){sentidoz = sentido2;}else{sentidoz = sentido1;}
        if((Dx == 0) && (Dy != 0) && (Dz != 0)){Md = max(abs(Dy),abs(Dz));}
        if((Dx == 0) && (Dy == 0) && (Dz != 0)){Md = abs(Dz);}
        if((Dx == 0) && (Dy != 0) && (Dz == 0)){Md = abs(Dy);}
        if((Dx != 0) && (Dy == 0) && (Dz != 0)){Md = max(abs(Dx),abs(Dz));}
        if((Dx != 0) && (Dy == 0) && (Dz == 0)){Md = abs(Dx);}
        if((Dx != 0) && (Dy != 0) && (Dz == 0)){Md = max(abs(Dx),abs(Dy));}
        if((Dx != 0) && (Dy != 0) && (Dz != 0)) {Md = max(abs(Dx),abs(Dz)); Md = max(Md,abs(Dy));}
        if((Dx == 0) && (Dy == 0) && (Dz == 0)){Md = 0;}
   
         while(1){
           if(digitalRead(END_HALL_SENSOR) == HIGH){
              XYZ_ENABLE(LOW);
              digitalWrite(X_DIR_PIN,sentidox);
              digitalWrite(Y_DIR_PIN,sentidoy);
              digitalWrite(Z_DIR_PIN,sentidoz);
                
              for(int kk = 0; kk < Md;kk++){
                if(kk < abs(Dx)){digitalWrite(X_STEP_PIN,HIGH);}
                if(kk < abs(Dy)){digitalWrite(Y_STEP_PIN,HIGH);}
                if(kk < abs(Dz)){digitalWrite(Z_STEP_PIN,HIGH);}
                delayMicroseconds(V_measure);
                digitalWrite(X_STEP_PIN,LOW);
                digitalWrite(Y_STEP_PIN,LOW);
                digitalWrite(Z_STEP_PIN,LOW);
                delayMicroseconds(V_measure);}
                
                
              //delay(tiempoDelay_s2);
              digitalWrite(START_HALL_SENSOR,HIGH);
              delay(1);
              digitalWrite(START_HALL_SENSOR,LOW);   
              break;}}
          break;}}
          
    x0 = x; y0 = y; z0 = z;}}
XYZ_ENABLE(HIGH);}


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
    
    
    
/* Motors Home */    
void Motors_Home(){ Y_Motor_Home(); X_Motor_Home(); Z_Motor_Home();} 


/*  X axis Motor Move 1 Step */
void X_Move_1_STEP(){
 // digitalWrite(X_ENABLE_PIN,HIGH);
  if((digitalRead(X_FRONT_PIN)==HIGH) && (digitalRead(X_BEHIND_PIN)==LOW)){
      digitalWrite(X_ENABLE_PIN,LOW);
      digitalWrite(X_DIR_PIN,sentido1);
      digitalWrite(X_STEP_PIN,HIGH);
      delayMicroseconds(V_manual);
      digitalWrite(X_STEP_PIN,LOW);
      delayMicroseconds(V_manual);
      x++;
  }
  if((digitalRead(X_FRONT_PIN)==LOW) && (digitalRead(X_BEHIND_PIN)==HIGH)){
      if(digitalRead(X_MIN_PIN)==LOW){
        digitalWrite(X_ENABLE_PIN,LOW);
        digitalWrite(X_DIR_PIN,sentido2);
        digitalWrite(X_STEP_PIN,HIGH);
        delayMicroseconds(V_manual);
        digitalWrite(X_STEP_PIN,LOW);
        delayMicroseconds(V_manual);
        x--;
      }
      if(digitalRead(X_MIN_PIN)==HIGH)x = 0;}}

/*  Y axis Motor Move 1 Step */
void Y_Move_1_STEP(){
 // digitalWrite(Y_ENABLE_PIN,HIGH);
  if((digitalRead(Y_FRONT_PIN)==HIGH) && (digitalRead(Y_BEHIND_PIN)==LOW)){
      digitalWrite(Y_ENABLE_PIN,LOW);
      digitalWrite(Y_DIR_PIN,sentido2);
      digitalWrite(Y_STEP_PIN,HIGH);
      delayMicroseconds(V_manual);
      digitalWrite(Y_STEP_PIN,LOW);
      delayMicroseconds(V_manual);
      y++;
  }
  if((digitalRead(Y_FRONT_PIN)==LOW) && (digitalRead(Y_BEHIND_PIN)==HIGH)){
      if(digitalRead(Y_MIN_PIN)==LOW){
        digitalWrite(Y_ENABLE_PIN,LOW);
        digitalWrite(Y_DIR_PIN,sentido1);
        digitalWrite(Y_STEP_PIN,HIGH);
        delayMicroseconds(V_manual);
        digitalWrite(Y_STEP_PIN,LOW);
        delayMicroseconds(V_manual);
        y--;}
      if(digitalRead(Y_MIN_PIN)==HIGH) y = 0;}}

/*  Z axis Motor Move 1 Step */
void Z_Move_1_STEP(){
 // digitalWrite(Z_ENABLE_PIN,HIGH);
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
             XYZ_ENABLE(HIGH);
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
  Motors_Home();
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
             XYZ_ENABLE(HIGH);
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
  Motors_Home();
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
             XYZ_ENABLE(HIGH);
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
  Motors_Home();
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
             XYZ_ENABLE(HIGH);
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
             
       Motors_Home();
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
             XYZ_ENABLE(HIGH);
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
        Motors_Home();
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
             XYZ_ENABLE(HIGH);
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
             
        Motors_Home();
}

// Take Data from One point
void Data_Point(){
  int p_x, p_y, p_z;

  for(p_x = 0;p_x<Point1[0];p_x++)  X_1_STEP(sentido1);// send X,Y,Z axis to Point1 
  for(p_z = 0;p_z<Point1[2];p_z++)  Z_1_STEP(sentido1);  
  for(p_y = 0;p_y<Point1[1];p_y++)  Y_1_STEP(sentido2);
   
  XYZ_ENABLE(HIGH);
  
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
      
    Motors_Home();
}
/*  Print coordinades*/
void Print_Direction(){
  if(digitalRead(PRINT_POINT) == LOW){
    float xd = x;
    float yd = y;
    float zd = z;
    xd = xd/160;
    yd = yd/201;
    zd = zd/201;
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

void Configuration(){
  Serial.begin(115200);
 Serial.println("Bienvenidos");
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

void READ_CONF_WORD(){
  if(Serial.available()>0){
    Serial.readBytesUntil('*',arranque,100);
    int len = sizeof(arranque);
    int h = 0, ff = 0, i, g;
    for(i = 0; i < len; i++){
      if(arranque[i]  == '@'){
        h = h + 1;
        switch(h){
          case 1:
            STAR    = String(AUX).toInt();
            for (g = 0; g<7; g++) {AUX[g] = '\0';}
            ff = 0;
            break;
          case 2:
            MODE    = String(AUX).toInt();
            for (g = 0; g<7; g++) {AUX[g] = '\0';}
            ff = 0;
            break;
            case 3:
            PX1     = String(AUX).toInt();
            for (g = 0; g<7; g++) {AUX[g] = '\0';}
            ff = 0;
            break;
          case 4:
            PY1     = String(AUX).toInt();
            for (g = 0; g<7; g++) {AUX[g] = '\0';}
            ff = 0;
            break;
          case 5:
            PZ1     = String(AUX).toInt();
            for (g = 0; g<7; g++) {AUX[g] = '\0';}
            ff = 0;
            break;
          case 6:
            PX2     = String(AUX).toInt();
            for (g = 0; g<7; g++) {AUX[g] = '\0';}
            ff = 0;
            break;
          case 7:
            PY2     = String(AUX).toInt();
            for (g = 0; g<7; g++) {AUX[g] = '\0';}
            ff = 0;
            break;
          case 8:
            PZ2     = String(AUX).toInt();
            for (g = 0;g<7;g++){AUX[g] = '\0';}
            ff = 0;
            break;
          case 9:
            R_factor = String(AUX).toInt();
            for (g = 0; g<7; g++) {AUX[g] = '\0';}
            ff = 0;
            break;
          case 10:
            V_measure = String(AUX).toInt();
            for (g = 0; g<7; g++) {AUX[g] = '\0';}
            ff = 0;
            break;
          case 11:
            V_manual     = String(AUX).toInt();
            for (g = 0; g<7; g++) {AUX[g] = '\0';}
            ff = 0;
            break;
          default:
            break;
          }
        }
       else
         AUX[ff++] = arranque[i];}}
}

void XYZ_ENABLE(boolean mode){
   digitalWrite(X_ENABLE_PIN,mode);
   digitalWrite(Y_ENABLE_PIN,mode);
   digitalWrite(Z_ENABLE_PIN,mode);}
   
   
void CHOOSE_MATRIZ(){
  
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
}
