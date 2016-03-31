char lon_val;
char lat_val;

double lonBuff;
double latBuff;

double Datatransfer(char *data_buf,char num)//convert the data to the float type
{                                           //*data_buf：the data array                                       
  double temp = 0.0;                           //the number of the right of a decimal point
  unsigned char i,j;
 
  if(data_buf[0]=='-')
  {
    i=1;
    //process the data array
    while(data_buf[i]!='.')
      temp=temp*10+(data_buf[i++]-0x30);
    for(j=0;j<num;j++)
      temp=temp*10+(data_buf[++i]-0x30);
    //convert the int type to the float type
    for(j=0;j<num;j++)
      temp=temp/10;
    //convert to the negative numbe
    temp=0-temp;
  }
  else//for the positive number
  {
    i=0;
    while(data_buf[i]!='.')
      temp=temp*10+(data_buf[i++]-0x30);
    for(j=0;j<num;j++)
      temp=temp*10+(data_buf[++i]-0x30);
    for(j=0;j<num;j++)
      temp=temp/10 ;
  }
  return temp;
}
 
char ID()//Match the ID commands
{ 
  char i=0;
  char value[6]={
    '$','G','P','G','G','A'      };//match the gps protocol
  char val[6]={
    '0','0','0','0','0','0'      };
 
  while(1)
  {
    if(Serial.available())
    {
      val[i] = Serial.read();//get the data from the serial interface
      if(val[i]==value[i]) //Match the protocol
      {    
        i++;
        if(i==6)
        {
          i=0;
          return 1;//break out after get the command
        }
      }
      else
        i=0;
    }
  } 
}
 
void comma(char num)//get ','
{   
  char val;
  char count=0;//count the number of ','
 
  while(1)
  {
    if(Serial.available())
    {
      val = Serial.read();
      if(val==',')
        count++;
    }
    if(count==num)//if the command is right, run return
      return;
  }
 
}

void UTC()//get the UTC data -- the time
{
  char i;
  char time[9]={
    '0','0','0','0','0','0','0','0','0'
  };
  double t=0.0;
 
  if( ID())//check ID
  {
    comma(1);//remove 1 ','
    //get the datas after headers
    while(1)
    {
      if(Serial.available())
      {
        time[i] = Serial.read();
        i++;
      }
      if(i==9)
      {
        i=0;
        t=Datatransfer(time,2);//convert data
        t=t-30000.00; //Halifax, NS
        long time=t;
        int h=time/10000;
        int m=(time/100)%100;
        int s=time%100;
        
//        if(h>=24)               //UTC+
//        {
//        h-=24;
//        }
 
         if (h<0)               //UTC-
        {
          h+=24;
        }

        File dataFile = SD.open("datalog.txt", FILE_WRITE);

        if (dataFile) {
          dataFile.print("UTC: ");
          dataFile.print(h);
          dataFile.print(":");
          dataFile.print(m);
          dataFile.print(":");
          dataFile.println(s);
          dataFile.close();
        }

        // if the file isn't open, pop up an error:
        else {
          Serial.println("error opening datalog.txt");
        }

        return;
      }  
    }
  }
}

void latitude()//get latitude
{
  char i;
  char lat[10]={'0','0','0','0','0','0','0','0','0','0'};
 
  if( ID())
  {
    comma(2);
    while(1)
    {
      if(Serial.available())
      {
        lat[i] = Serial.read();
        i++;
      }
      if(i==10)
      {
        i=0;
        
        File dataFile = SD.open("datalog.txt", FILE_WRITE);

        if (dataFile) {
          dataFile.print("Lat: ");
          latBuff = Datatransfer(lat,5)/100.0;
          dataFile.println(Datatransfer(lat,5)/100.0,7);  
          dataFile.close();
        }

        // if the file isn't open, pop up an error:
        else {
          Serial.println("error opening datalog.txt");
        }

        return;
      }  
    }
  }
}

void lat_dir()//get dimensions
{
  char i=0;
 
  if( ID())
  {
    comma(3);
    while(1)
    {
      if(Serial.available())
      {
        lat_val = Serial.read();

        File dataFile = SD.open("datalog.txt", FILE_WRITE);

        if (dataFile) {
          dataFile.print("Dir: ");
          dataFile.println(lat_val);
          dataFile.close();
        }

        // if the file isn't open, pop up an error:
        else {
          Serial.println("error opening datalog.txt");
        }

        i++;
      }
      if(i==1)
      {
        i=0;
        return;
      }  
    }
  }
}

void longitude()//get longitude
{
  char i;
  char lon[11]={'0','0','0','0','0','0','0','0','0','0','0'};
 
  if( ID())
  {
    comma(4);
    while(1)
    {
      if(Serial.available())
      {
        lon[i] = Serial.read();
        i++;
      }
      if(i==11)
      {
        i=0;

        File dataFile = SD.open("datalog.txt", FILE_WRITE);

        if (dataFile) {
          dataFile.print("Lon: -");
          lonBuff = (Datatransfer(lon,5)/100.0);
          dataFile.println(Datatransfer(lon,5)/100.0,7);
          dataFile.close();
        }

        // if the file isn't open, pop up an error:
        else {
          Serial.println("error opening datalog.txt");
        }

        return;
      }  
    }
  }
}

void lon_dir()//get direction data
{
  char i=0;
 
  if( ID())
  {
    comma(5);
    while(1)
    {
      if(Serial.available())
      {
        lon_val = Serial.read();

        File dataFile = SD.open("datalog.txt", FILE_WRITE);

        if (dataFile) {
          dataFile.print("Dir: ");
          dataFile.println(lon_val);
          dataFile.close();
        }

        // if the file isn't open, pop up an error:
        else {
          Serial.println("error opening datalog.txt");
        }

        i++;
      }
      if(i==1)
      {
        i=0;
        return;
      }  
    }
  }
}

void altitude()//get altitude data
{
  char i,flag=0;
  char alt[8]={
    '0','0','0','0','0','0','0','0'
  };
 
  if( ID())
  {
    comma(9);
    while(1)
    {
      if(Serial.available())
      {
        alt[i] = Serial.read();
        if(alt[i]==',')
          flag=1;
        else
          i++;
      }
      if(flag)
      {
        i=0;

        File dataFile = SD.open("datalog.txt", FILE_WRITE);

        if (dataFile) {
          dataFile.print("Alt: ");
          dataFile.println(Datatransfer(alt,1),1);
          dataFile.close();
        }

        // if the file isn't open, pop up an error:
        else {
          Serial.println("error opening datalog.txt");
        }
        
        return;
      }  
    }
  }
}
