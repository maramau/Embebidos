/**
 * Clase que define un graficador de funciones que hace scroll horizontal en el tiempo.
 * Permite definir un conjunto arbitrario de funciones y plotearlas en el mismo gráfico.
 * El scroll del gráfico es controlado por las modificaciones a los valores del mismo (si no
 * se producen actualizaciones en los valores de las funciones, el gráfico no se desplaza).

 */
 
class ScrollingFcnPlot
{
  float[] f;
  color colorF;
  int fPtr;
  int cantValues;
  boolean mustScroll = false;
  float maxVal, minVal;
  
  ScrollingFcnPlot (int cantValues, color colorF, float minVal, float maxVal)
  {
    this.f = new float[cantValues];
    this.fPtr = 0;
    this.cantValues = cantValues;
    this.colorF = colorF;
    this.maxVal = maxVal;
    this.minVal = minVal;    
  }
  
  void updateValue(float value)
  {
    this.f[fPtr] = value;
    this.mustScroll = true;
      
    //Se incrementa el puntero circular
    fPtr = (fPtr + 1) % cantValues;
  }
  
  void updateMax(float max){
    this.maxVal = max;
  }
  void updateMin(float min){
    this.minVal = min;
  }
  
  void displayIntoRect(int x1,int y1,int x2,int y2)
  {
    if (mustScroll)
    {
      mustScroll = false;
      
      stroke(colorF);
      strokeWeight(4);
      beginShape();
      for(int i = 0; i<f.length;i++){
        vertex(map(i,0,cantValues,x1,x2),map(f[(fPtr+i)%cantValues], minVal, maxVal, y2, y1));
      }
      endShape();
    }  
  }
}
