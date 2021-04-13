import processing.serial.*;
/**
 * Ejemplo de skecth Processing para el desarrollo del Laboratorio.
 *
 * Este skecth implementa un programa en el host que tiene la capacidad de
 * graficar funciones que evolucionan en el tiempo y además permite la
 * presentación de datos simples mediante Labels, y la captura de eventos
 * del usuario mediante la implementación de botones simples.
 *
 * La aplicación divide la ventana en 2 regiones, una de dibujado y otra 
 * donde se ubican los botones y etiquetas de información.
 *
 */

static final int SALTO =  10;  //Codigo ASCII del salto de linea

Serial myPort;
String buffer;

int maximo= 40;
int minimo= 10;
int valores;

float tempActual = 0;
float tempMin = 0;
float tempMax = 0;
float tempPromedio = 0;
float estado = 0;

// Declaraciones para graficar funciones...
ScrollingFcnPlot fActual,fMin,fMax,fPromedio;

//Botones...
RectButton botonModo;

boolean prealert = false;

Label labelModo, modo, labelActual, actual, labelMax, max, labelMin, min, labelProm, promedio;
Label maxVal,midVal,minVal;

//Ventana y viewports...
int pgFunctionViewportWidth = 800;
int pgControlViewportWidth = 100;
int pgViewportsHeight = 500;

void setup() {
  
  myPort = new Serial(this, Serial.list()[0], 9600);

  //Se define el tamaño de la ventana de la aplicación... 
  size(950, 500);
  
  //Se inicializan los arreglos para almacenar las funciones...
  valores = pgFunctionViewportWidth;
  fActual = new ScrollingFcnPlot(valores,color(50,50,50),minimo,maximo);
  fMax = new ScrollingFcnPlot(valores,color(255,0,0),minimo,maximo);
  fMin = new ScrollingFcnPlot(valores,color(0,0,255),minimo,maximo);
  fPromedio = new ScrollingFcnPlot(valores,color(0,255,0),minimo,maximo);
  
  //Se inicializan los botones...
  botonModo = new RectButton(pgFunctionViewportWidth+10,10,100,40,color(255),color(200),color(0),"MODO");
  
  //Se inicializan los labels...
  labelModo = new Label(pgFunctionViewportWidth+10,110,color(0),"Modo: ");
  modo = new Label(pgFunctionViewportWidth+60,110,color(0),"-");
  labelActual = new Label(pgFunctionViewportWidth+10,130,color(50,50,50),"T.Actual: ");
  actual = new Label(pgFunctionViewportWidth+70,130,color(50,50,50),"-");
  labelMax = new Label(pgFunctionViewportWidth+10,150,color(255,0,0),"T.Max: ");
  max = new Label(pgFunctionViewportWidth+70,150,color(255,0,0),"-");
  labelMin = new Label(pgFunctionViewportWidth+10,170,color(0,0,255),"T.Min: ");
  min = new Label(pgFunctionViewportWidth+70,170,color(0,0,255),"-");
  labelProm = new Label(pgFunctionViewportWidth+10,190,color(0,255,0),"T.Prom: ");
  promedio = new Label(pgFunctionViewportWidth+70,190,color(0,255,0),"-");
  
  maxVal = new Label(3,5,color(0),String.valueOf(maximo));
  midVal = new Label(3,(pgViewportsHeight-20)/2,color(0),String.valueOf((maximo+minimo)/2));
  minVal = new Label(3,pgViewportsHeight-25,color(0),String.valueOf(minimo));
}

String modo(float estado){
  int estadoNormalizado = (int) estado;
  switch (estadoNormalizado) {
    case 0 :
      return "ACTUAL";
    case 1 :
      return "MINIMO";
    case 2 :
      return "MAXIMO";
    case 3 :
      return "PROMEDIO";
    default:
      return "";
  }
}

boolean isFull(String buffer){
  boolean toReturn = true;
  if(!(buffer.charAt(0) == '(') ||  !(buffer.charAt(buffer.length() - 3) == ')')){
    toReturn = false;
  }
  return toReturn;
}

float[] procesarDatos(String buffer){
  String[] items = buffer.replaceAll("\\(", "").replaceAll("\\)", "").replaceAll("\\s", "").split("/");

  float[] results = new float[items.length];
  
  for (int i = 0; i < items.length; i++) {
      results[i] = Float.parseFloat(items[i]);
  }
  return results;
}

void draw() {
  background(240);
  stroke(0);
  noFill();
  
  //Permite expermientar con la velocidad de scroll al actualizar más lentamente los valores...
  delay(50);

  if (myPort.available() > 0){
    buffer = myPort.readStringUntil(SALTO); //leo hasta el salto de linea
  }
  if(buffer != null){
    if(isFull(buffer)){
      float[] temperaturas = procesarDatos(buffer);
      if(temperaturas.length == 5){
        
        tempActual = temperaturas[0];
        tempMin = temperaturas[1];
        tempMax = temperaturas[2];
        tempPromedio = temperaturas[3];
        estado = temperaturas[4];
        
        actual.caption= Float.toString(tempActual);
        min.caption= Float.toString(tempMin);
        max.caption= Float.toString(tempMax);
        promedio.caption=Float.toString(tempPromedio);
        modo.caption=modo(estado);
        
      }
    }
  }
  
  //Actualizo los proximos valores de las funciones
  fActual.updateValue(tempActual);
  fMin.updateValue(tempMin);
  fMax.updateValue(tempMax);
  fPromedio.updateValue(tempPromedio);
  
  //Dibujo las funciones
  fMin.displayIntoRect(30,10,pgFunctionViewportWidth-10,pgViewportsHeight-10);
  fMax.displayIntoRect(30,10,pgFunctionViewportWidth-10,pgViewportsHeight-10);
  fPromedio.displayIntoRect(30,10,pgFunctionViewportWidth-10,pgViewportsHeight-10);
  fActual.displayIntoRect(30,10,pgFunctionViewportWidth-10,pgViewportsHeight-10);
  
  //Proceso los eventos de MouseOver
  botonModo.update();
  
  //Procesa las entradas (botones)
  if(mousePressed) {
    if(botonModo.pressed()) {
      botonModo.currentcolor = color(200,100,200);
      prealert = true;
    }
  }
  
  
  //Dibuja el eje X y el recuadro de los gráficos...
  stroke(0);
  line(30, pgViewportsHeight/2, pgFunctionViewportWidth-10, pgViewportsHeight/2);
  rect(30,10,pgFunctionViewportWidth-40,pgViewportsHeight-20);
  
  //Se dibujan los botones...
  botonModo.display();
  
  //Se dibuja texto adicional...(labels, etc)
  labelModo.display();
  modo.display();
  labelActual.display();
  actual.display();
  labelMax.display();
  max.display();
  labelMin.display();
  min.display();
  labelProm.display();
  promedio.display();
  
  maxVal.display();
  midVal.display();
  minVal.display();
  
}

void mouseReleased()
{
  //Si se pulsó algún botón y se completa el click, se envia cambio de modo...
  if(prealert)
  {
    myPort.write('X');
    prealert = false;
  }
}
