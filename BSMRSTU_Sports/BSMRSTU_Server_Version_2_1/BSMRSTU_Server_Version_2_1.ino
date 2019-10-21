/*
 * -------------------BSMRSTU Sports Server---------------------------------
 * ............Programmer : Chandan Kumar Mondal............................
 .....................ID : 20151102053.....................................
 .........................ETE,BSMRSTU....................................
 * --------------------------version:2.1----------------------------------

 */


// Library Section..............................................
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <Adafruit_ST7789.h> 
#include <SPI.h>
#include <Fonts/chandan_special_font_15.h> //Rochester_Regular_30
//----------------------------------------------------------------

// Object Section...............................................
DHT ck(D3, DHT22);
ESP8266WebServer server(80);
Adafruit_ST7735 tft = Adafruit_ST7735(D2,  D4, D1);
//----------------------------------------------------------------

// Variable declaration.........................................
#define SSID "Chandan"
#define PASS "ssskac53"
int led = D0;
int green = D6;
int blue = D8;
String firstteam = "";
String Secondteam = "";
int teamA = 0;
int teamB = 0;
int runsTeamA = 0;
int runsTeamB = 0;
int wicketTeamA = 0;
int wicketTeamB = 0;
int first_team = 10;
int Second_team = 11;
int currentTeam = first_team;
int last_press = 0;
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
//----------------------------------------------------------------

//------Boolean Varialbles---------------------------------------
boolean configure = false;
boolean cricket = false;
boolean football = false;
boolean badminton = false;
boolean team = false;

boolean firststep = false;
boolean secondstep = false;
boolean final_step = false;
boolean matrix_mode = false;
//------------------------------------------------------------------




//Messae Handle Section.........................................
void handle_msg() {
  server.send(200, "text/html", myform());
  String msg = server.arg("msg");
  String non_orginal = msg;

  non_orginal.toLowerCase();
  if(non_orginal=="eteian speaking"){firststep=true;}
  if(non_orginal=="chandan"){if(firststep){secondstep=true;}}
  if(non_orginal=="20151102053")
  {
    if(firststep && secondstep)
    {
      final_step = true;
    }
  }

  if(final_step)
  {
    if(non_orginal=="led on"){digitalWrite(led,HIGH); }
    else if(non_orginal=="led off"){digitalWrite(led,LOW); msg="";}
    else if(non_orginal=="end"){firststep=false; secondstep=false;final_step=false;msg="";}
  }

  if(non_orginal=="matrix on"){
      matrix_mode=true;
  }
  if(non_orginal=="matrix off")
  {
    matrix_mode =false;
  }

  if(matrix_mode)
  {
    msg+='\n';
    char carrier[50];
    msg.toCharArray(carrier,50);
    Serial.write(carrier);
  }
}
//---------------------------------------------------------------------------------------------------------



void setup(void) {
  Serial.begin(9600);
  ck.begin();
  pinMode(led, OUTPUT);
  
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  
  int warning = 0;

  WiFi.begin(SSID, PASS);

tft.setRotation(2);
  tft.setCursor(5, 50);
  tft.print("Connecting");
  int mycursor = 70;

  while (WiFi.status() != WL_CONNECTED) {
      tft.setCursor(mycursor++, 50);
      tft.print(".");
      delay(100);
      if (mycursor > 99)
      {
        tft.fillRect(69, 45, 35, 20, BLACK);
        mycursor=70;
      }
  }
  //-------------------------------------------------
  IPAddress ip(192, 168, 137, 20);              //|
  IPAddress gateway(192, 168, 137, 254);        //|
  IPAddress subnet(255, 255, 255, 0);           //|
  WiFi.config(ip, gateway, subnet);             //|
  //-------------------------------------------------

  server.on("/", []() {
    server.send(200, "text/html", myform());
  });
  server.on("/msg", handle_msg);
  server.begin();

    tft.fillScreen(BLACK);
    tft.setFont(&Rochester_Regular_15);
    tft.setCursor(15,60);
    tft.setTextColor(GREEN);
    tft.println("CONNECTED");
    tft.println();
    tft.setFont();
    tft.print("  IP : 192.168.137.20");
    delay(2000);
    tft.fillScreen(BLACK);

    gameConfigure();

}


char upcoming;

void loop(void) {

  server.handleClient();



  if (Serial.available() > 0)
  {
    upcoming = Serial.read();

    if (upcoming == '1')
    {
      if (football || badminton)
      {
        teamA += 1;
        last_press = 1;
      }
      if (cricket)
      {
        currentTeam == first_team ? runsTeamA += 1 : runsTeamB += 1;
      }
    }

    else if (upcoming == '7')
    {
      if (football || badminton)
      {
        teamA -= 1;
        last_press = 1;

        teamA < 0 ? teamA = 0 : teamA = teamA;
      }

      if (cricket)
      {
        currentTeam == first_team ? runsTeamA -= 1 : runsTeamB -= 1;
        runsTeamA < 0 ? runsTeamA = 0 : runsTeamA = runsTeamA;
        runsTeamB < 0 ? runsTeamB = 0 : runsTeamB = runsTeamB;
      }
    }

    else if (upcoming == '2')
    {
      if (cricket)
      {
        currentTeam == first_team ? runsTeamA += 2 : runsTeamB += 2;
      }
    }

    else if (upcoming == '3')
    {
      if (football || badminton)
      {
        teamB += 1;
        last_press = 2;
      }
      if (cricket)
      {
        currentTeam == first_team ? runsTeamA += 3 : runsTeamB += 3;
      }
    }

    else if (upcoming == '4')
    {
      if (cricket)
      {
        currentTeam == first_team ? runsTeamA += 4 : runsTeamB += 4;
      }
    }

    else if (upcoming == '5')
    {
      if (cricket)
      {
        currentTeam == first_team ? runsTeamA += 5 : runsTeamB += 5;
      }
    }

    else if (upcoming == '6')
    {
      if (cricket)
      {
        currentTeam == first_team ? runsTeamA += 6 : runsTeamB += 6;
      }
    }

    else if (upcoming == '9')
    {
      if (football || badminton)
      {
        teamB -= 1;
        last_press = 2;
        teamB < 0 ? teamB = 0 : teamB = teamB;
      }
    }

    else if (upcoming == '0')
    {
      if (cricket)
      {
        currentTeam == first_team ? currentTeam = Second_team : currentTeam = first_team;
      }
    }

    else if (upcoming == '#')
    {
      ShowTheIP();
    }

    else if (upcoming == 'D')
    {
      if (badminton || football)
      {
        last_press == 1 ? teamA = 0 : teamB - 0;
      }
      if (cricket)
      {
        currentTeam == first_team ? teamA = 0 : teamB = 0;
      }
    }

    else if (upcoming == 'A')
    {
      if (cricket)
      {
        currentTeam == first_team ? wicketTeamA += 1 : wicketTeamB += 1;
      }
    }

    else if (upcoming == 'B')
    {
      if (cricket)
      {
        currentTeam == first_team ? wicketTeamA -= 1 : wicketTeamB -= 1;
        wicketTeamA < 0 ? wicketTeamA = 0 : wicketTeamA = wicketTeamA;
        wicketTeamB < 0 ? wicketTeamB = 0 : wicketTeamB = wicketTeamB;
      }
    }
    else if (upcoming == 'C')
    {
        tft.fillScreen(BLACK);
        tft.setCursor(5,40);
        tft.setTextColor(RED);
        tft.setTextSize(1);
        tft.println("Warning: ");
        tft.setTextColor(WHITE);
        tft.println("Sure ? data will be lost");
        tft.println();
        tft.setTextColor(BLUE);
        tft.println(" Press * for Confirm");
        tft.println(" Press C for Cancel");

      boolean waiting = true;
      char mycommand;
      while (waiting)
      {
        if (Serial.available() > 0)
        {
          mycommand = Serial.read();

          if (mycommand == '*')
          {
            waiting = false;
            runsTeamA = 0;
            teamA = 0;
            teamB = 0;
            runsTeamB = 0;
            wicketTeamA = 0;
            wicketTeamB = 0;
            tft.fillScreen(BLACK);
            gameConfigure();
          }
          else if (mycommand == 'C')
          {
            waiting = false;
            tft.fillScreen(BLACK);
            if (cricket)
            {
              showTheC();
            }
            else if (badminton || football)
            {
              showTheFB();
            }
            ScoreBoard();
          }
        }
      }
    }
    ScoreBoard();
  }
}

String myform() {
  String form =
    "<!DOCTYPE html>"
    "<html>"
    "<head>";
    if(!final_step){ form+="<meta http-equiv=\"refresh\" content=\"15\" >" ;}
    form+="<style>"

    "body{padding: 0; margin: 0;  background:";
    final_step == true? form+="linear-gradient(130deg, rgb(177, 177, 58) ,rgb(32, 29, 29), rgb(182, 40, 76), rgb(226, 148, 30));}" : form+="linear-gradient(130deg, rgb(59, 59, 55) ,rgb(185, 19, 19), rgb(45, 94, 201), rgb(48, 46, 43));}";
    form+="h1{background:";
    final_step==true? form+=" linear-gradient(130deg, rgb(122, 122, 35) ,rgb(90, 8, 8), rgb(78, 25, 85), rgb(168, 30, 48));" : form+="green;";
    if(final_step)
    {
      form+="animation: animate 4s infinite; background-size:400%;";
    }
    form+= "transition: 0.5s all; color: white; text-align: center; border: 3px dotted white; border-radius: 30px; margin: 10px 20px;}";

    if(final_step)
    {
      form+="@keyframes animate{ 0%{background-position:0%; letter-spacing:0px;} 20%{background-position:50%; letter-spacing:5px;} 40%{background-position: 100%; letter-spacing:10px;} 60%{background-position:200%; letter-spacing:10px;} 80%{background-position: 300%; letter-spacing:5px;} 100%{background-position:400%; letter-spacing:0px;}}";
    }else{
      form+=" h1:hover{ background: white; color: black; letter-spacing: 20px; border: 3px dotted black; }";
    }

    form+=".display{ height: 100px; width: 200px; float: left; transition: 0.5s all ease; margin-left: 30px; background: #789654;; border: 3px solid black; border-radius: 30px; }"
    ".display:hover{border: 5px dotted black; background: #451691; }"
    ".humd{ height: 30px; width: 180px; transition: 0.5s ease; background : #bacd; position: absolute; margin: 10px 10px; text-align: center; font-size: 28px; }"
    ".humd:hover{ background: green; border: 2px solid black; text-transform: uppercase; overflow: hidden; border-radius: 10px; color: white; font-family: 'Times New Roman', Times, serif; }"
    ".temp{ height: 30px; width: 180px; background: #ccffff; position: absolute; margin-left: 10px; margin-top : 60px; text-align: center; font-size: 28px; }"
    ".temp:hover{ background: green; border: 2px solid black; text-transform: uppercase; overflow: hidden; border-radius: 10px; color: white; font-family: 'Times New Roman', Times, serif; }"
    ".choice{ height: 100px; width: 400px; opacity: 0.7; margin-top: 30px; margin-left: 65%; }"
    ".choice:hover{ opacity: 1; }"
    " form{ position: absolute; height: 50px; width:400px; background:";
    final_step==true? form+="linear-gradient(130deg, rgb(209, 209, 188) ,rgb(209, 18, 18), rgb(78, 25, 85), rgb(30, 168, 65));":form+=" #123456;";
    form+= " outline: 5px solid black; }"
    " .textsec{ margin-top: 10px; margin-left: 30px; border: 3px dotted black; border-radius: 10px; }"
    " b{ margin-left:10px; }"
    " .submitIt{ background: #000fff; color: black; transition: 0.5s ease; margin-top: 10px; margin-left: 20px; border-radius: 10px; border: 3px dotted black; font-family: 'Times New Roman', Times, serif; font-size: 20px; }"
    " .submitIt:hover{ background: black; color:white; letter-spacing: 3px; }"
    " p{ opacity: 1; text-align: center; font-family: Blackoak std; }"
    ".about{height: 80px;width:300px;float: right;margin-left: 30px;margin-right: 30px; margin-top:20px;margin-bottom:10px;color: black;background: white;text-align: center;border: 4px dotted black;opacity: 0.7;}"
    ".instruction{height: 300px;width: 900px; margin-top: 20px;margin-left: 200px;background:";
    final_step==true? form+="linear-gradient(130deg, rgb(122, 122, 35) ,rgb(90, 8, 8), rgb(78, 25, 85), rgb(168, 30, 48)); border: 3px solid black;border-radius: 15px;}" : form+="linear-gradient(30deg, #12ffdd, #ab3455, #abcdef, #ccffff);border: 3px solid black;border-radius: 15px;}";
    form+="article{margin-left: 10px; margin-top: 5px;}"
    "marquee{background: black; color: white;transition: 0.5s ease;font-size: 20px;margin-top: 5px; margin-right: 10px;border: 2px dotted white;border-radius: 10px;}"
    "marquee:hover{font-size: 40px; background:blue;}"
    " .foot{text-decoration: none;text-align: center;background: green;margin-top: 20px;margin-right: 200px;margin-left: 200px;text-transform: uppercase;letter-spacing: 10px;color: white;font-size: 20px;padding: 10px;border-radius: 15px;font-family: Copperplate Gothic Bold;}"

    "</style>"
    "</head>"


    "<body>"
    "<h1>";
    final_step==true? form+="Chandan's Appliences": form+="BSMRSTU Sports";    
    form+="</h1>"
    "</br>"
    "<div class=\"display\">"
    "<div class=\"temp\">"
    "<i>";

          form += String(int(ck.readTemperature()));
          form += " deg C</i>"
    "</div>"

      "<div class=\"humd\">"
          "<i>";
          form += String(int(ck.readHumidity()));
          form += " % humidity</i>"
          "</div>"
          "</div>"

          "<div class=\"choice\">"
          "<form action=\"msg\">"
          "<b>Message</b>"
          "<input class=\"textsec\" type=\"text\" name=\"msg\" size: 50px autofocus/>"
          "<input class=\"submitIt\" type=\"submit\" value=\"Submit\"/>"
          "</form>"
          "</div>"

          "<div class=\"instruction\">"
          "<article>"
          "<div class=\"foot\">football</div>";
  if (football) {
    form += "<marquee>";
    form += firstteam;
    form += " ";
    form += teamA;
    form += " - ";
    form += Secondteam;
    form += " ";
    form += teamB;
    form += "</marquee>";
  }


  form += "<div class=\"foot\">Cricket</div>";
  if (cricket) {
    form += "<marquee>";
    currentTeam == first_team ? form += "*" : form += "";
    form += firstteam;
    form += " ";
    form += "Runs : ";
    form += runsTeamA;
    form += " / ";
    form += wicketTeamA;
    form += "     ";
    currentTeam == Second_team ? form += "*" : form += "";;
    form += Secondteam;
    form += " ";
    form += "Runs : ";
    form += runsTeamB;
    form += " / ";
    form += wicketTeamB;
    form += "</marquee>";
  }


  form += "<div class=\"foot\">Badminton</div>";
  if (badminton) {
    form += "<marquee>";
    form += firstteam;
    form += " ";
    form += teamA;
    form += " - ";
    form += Secondteam;
    form += " ";
    form += teamB;
    form += "</marquee>";
  }
  form += "</article>"
          "</div>"

          "<div class=\"about\"><u> Programmed by : </u><code>Chandan Kumar Mondal</code><address> chandankumarckbd@gmail.com</address><address> ETE, 20151102053</address><address> BSMRSTU</address></div>"
          "</body>"
          "</html>";

  return form;
}

void clc()
{
  
}

void gameConfigure()
{
  tft.fillScreen(BLACK);
  tft.setCursor(10,20);
  tft.setFont();
  tft.setTextColor(RED);
  tft.setTextSize(1);
  tft.println("CONFIGURE");
  tft.println();
  tft.setTextColor(GREEN);
  tft.println("  A - Football");
  tft.println();
  tft.println("  B - Badminton");
  tft.println();
  tft.println("  C - Cricket");
  
  boolean GAME = true;
  char cmd;
  while (GAME) {
    if (Serial.available() > 0)
    {
      cmd = Serial.read();

      if (cmd == 'C')
      {
        tft.fillScreen(BLACK);
        tft.setCursor(5,40);
        tft.setTextColor(RED);
        tft.println("Warning: ");
        tft.setTextColor(WHITE);
        tft.println("You Select Cricket");
         tft.println();
        tft.setTextColor(BLUE);
        tft.println(" Press * for Confirm");
        tft.println(" Press C for Cancel");
        
        boolean waiting = true;
        char mychar;
        while (waiting)
        {
          if (Serial.available() > 0)
          {
            mychar = Serial.read();
            if (mychar == 'C')
            {
              waiting = false;
              gameConfigure();
            }
            else if (mychar == '*')
            {
              waiting = false;
              GAME = false;
              cricket = true;
              football = false;
              badminton = false;
              teamSelection();
            }
          }
        }
      }
      else if (cmd == 'A')
      {
        tft.fillScreen(BLACK);
        tft.setCursor(5,40);
        tft.setTextColor(RED);
        tft.println("Warning: ");
        tft.setTextColor(WHITE);
        tft.println("You Select Football");
         tft.println();
        tft.setTextColor(BLUE);
        tft.println(" Press * for Confirm");
        tft.println(" Press C for Cancel");
        boolean waiting = true;
        char mychar;
        while (waiting)
        {
          if (Serial.available() > 0)
          {
            mychar = Serial.read();
            if (mychar == 'C')
            {
              waiting = false;
              gameConfigure();
            }
            else if (mychar == '*')
            {
              waiting = false;
              GAME = false;
              cricket = false;
              football = true;
              badminton = false;
              teamSelection();
            }
          }
        }
      }
      else if (cmd == 'B')
      {
        tft.fillScreen(BLACK);
        tft.setCursor(5,40);
        tft.setTextColor(RED);
        tft.println("Warning: ");
        tft.setTextColor(WHITE);
        tft.println("You Select Badminton");
         tft.println();
        tft.setTextColor(BLUE);
        tft.println(" Press * for Confirm");
        tft.println(" Press C for Cancel");
        boolean waiting = true;
        char mychar;
        while (waiting)
        {
          if (Serial.available() > 0)
          {
            mychar = Serial.read();
            if (mychar == 'C')
            {
              waiting = false;
              gameConfigure();
            }
            else if (mychar == '*')
            {
              waiting = false;
              GAME = false;
              cricket = false;
              football = false;
              badminton = true;
              teamSelection();
            }
          }
        }
      }
    }
  }
}

void teamSelection()
{
  tft.fillScreen(BLACK);
  tft.setCursor(10,20);
  tft.setTextColor(MAGENTA);
  tft.print("TEAMS : ");

  int num = 1;
  printDept(num);
  firstteam = "";
  Secondteam = "";
  boolean wait = true;
  char command;
  int cur = 5;
  boolean first_team_done = false;
  while (wait) {
    if (Serial.available() > 0)
    {
      command = Serial.read();
      if (command == '1')
      {
        if (num == 1)
        {
          first_team_done == false ? firstteam = "ETE" : Secondteam = "ETE";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 2)
        {
          first_team_done == false ? firstteam = "ART" : Secondteam = "ART";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 3)
        {
          first_team_done == false ? firstteam = "MAT" : Secondteam = "MAT";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 4)
        {
          first_team_done == false ? firstteam = "PHR" : Secondteam = "PHR";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 5)
        {
          first_team_done == false ? firstteam = "BAN" : Secondteam = "BAN";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 6)
        {
          first_team_done == false ? firstteam = "SOC" : Secondteam = "SOC";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 7)
        {
          first_team_done == false ? firstteam = "MAG" : Secondteam = "MAG";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 8)
        {
          first_team_done == false ? firstteam = "LAW" : Secondteam = "LAW";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }

      }

      else if (command == '2')
      {
        if (num == 1)
        {
          first_team_done == false ? firstteam = "CSE" : Secondteam = "CSE";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 3)
        {
          first_team_done == false ? firstteam = "STA" : Secondteam = "STA";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 4)
        {
          first_team_done == false ? firstteam = "BGE" : Secondteam = "BGE";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 5)
        {
          first_team_done == false ? firstteam = "ENG" : Secondteam = "ENG";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 6)
        {
          first_team_done == false ? firstteam = "IR " : Secondteam = "IR ";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 7)
        {
          first_team_done == false ? firstteam = "AIS" : Secondteam = "AIS";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 8)
        {
          first_team_done == false ? firstteam = "AGR" : Secondteam = "AGR";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
      }
      else if (command == '3')
      {
        if (num == 1)
        {
          first_team_done == false ? firstteam = "EEE" : Secondteam = "EEE";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 3)
        {
          first_team_done == false ? firstteam = "CHE" : Secondteam = "CHE";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 4)
        {
          first_team_done == false ? firstteam = "BMB" : Secondteam = "BMB";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 6)
        {
          first_team_done == false ? firstteam = "ECO" : Secondteam = "ECO";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 7)
        {
          first_team_done == false ? firstteam = "MKT" : Secondteam = "MKT";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
      }
      else if (command == '4')
      {
        if (num == 1)
        {
          first_team_done == false ? firstteam = "ACCE" : Secondteam = "ACCE";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 3)
        {
          first_team_done == false ? firstteam = "PHY" : Secondteam = "PHY";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 4)
        {
          first_team_done == false ? firstteam = "PSY" : Secondteam = "PSY";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 6)
        {
          first_team_done == false ? firstteam = "POS" : Secondteam = "POS";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 7)
        {
          first_team_done == false ? firstteam = "FAB" : Secondteam = "FAB";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
      }
      else if (command == '5')
      {
        if (num == 1)
        {
          first_team_done == false ? firstteam = "CE " : Secondteam = "CE ";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 3)
        {
          first_team_done == false ? firstteam = "ESD" : Secondteam = "ESD";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 4)
        {
          first_team_done == false ? firstteam = "BOT" : Secondteam = "BOT";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 7)
        {
          first_team_done == false ? firstteam = "THM" : Secondteam = "THM";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
      }
      else if (command == '6')
      {
        if (num == 1)
        {
          first_team_done == false ? firstteam = "FAE" : Secondteam = "FAE";
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
      }
      else if (command == '*')
      {
        if (cricket)
        {
          tft.fillScreen(BLACK);
          showTheC();
          wait = false;
        }
        if (football || badminton)
        {
          tft.fillScreen(BLACK);
          showTheFB();
          wait = false;
        }
        ScoreBoard();
        return;
      }
      else if (command == 'C')
      {
        first_team_done == true ? first_team_done = false : first_team_done = true;
        firstteam = "";
        printTheTeam(firstteam,Secondteam);
      }
      else if (command == '#')
      {
        num++;
        num > 8 ? num = 1 : num = num;
        printDept(num);
      }
    }
  }
}


void ShowTheIP()
{
  tft.setCursor(60,145);
  tft.print("IP: 192.168.137.20");

}

void showTheC()
{
  tft.fillScreen(BLACK);
  tft.setCursor(10,130);
  tft.println("1 to 6 Run.. A wicket up");;
  tft.println();
  tft.println("B wicket down.. 0-switch");
}

void showTheFB()
{
  tft.fillScreen(BLACK);
  tft.setCursor(10,130);
  tft.println("TeamA : 1 inc 7 dec");
  tft.println();
  tft.println("TeamB : 3 inc 7 dec");
  
}

void ScoreBoard()
{
  if (cricket)
  {
    tft.fillRect(5,35,122,25,BLACK);
    tft.setCursor(50,10);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    tft.print(firstteam);
    tft.print(" : ");
    tft.setTextColor(WHITE);
    tft.setCursor(10,35);
    tft.print(runsTeamA);
    tft.print(" / ");
    tft.println(wicketTeamA);
 
    if(currentTeam==first_team)
    {
      tft.setCursor(30,5);
      tft.setTextSize(1);
      tft.setTextColor(WHITE);
      tft.print("BAT");
    }else{
      tft.setCursor(30,5);
      tft.setTextSize(1);
      tft.setTextColor(BLACK);
      tft.print("BAT");
      tft.setTextColor(WHITE);
    }

    tft.fillRect(5,85,122,30,BLACK);
    tft.setCursor(50,60);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    tft.print(Secondteam);
    tft.print(" : ");
    tft.setCursor(10,85);
    tft.setTextColor(WHITE);
    tft.print(runsTeamB);
    tft.print(" / ");
    tft.println(wicketTeamB);
 

    if(currentTeam==Second_team)
    {
       tft.setCursor(30,55);
      tft.setTextSize(1);
      tft.setTextColor(WHITE);
      tft.print("BAT");
    }else{
      tft.setCursor(30,55);
      tft.setTextSize(1);
      tft.setTextColor(BLACK);
      tft.print("BAT");
      tft.setTextColor(WHITE);
    }
  }

  else if (badminton || football)
  {
    tft.fillRect(40,0,80,100,BLACK);
    tft.setCursor(5,10);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    tft.print(firstteam);
    tft.print(" : ");
    tft.setTextColor(WHITE);
    tft.println (teamA);

    tft.setCursor(5,50);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    tft.print(Secondteam);
    tft.print(" : ");
    tft.setTextColor(WHITE);
    tft.print(teamB);
  }
}

void printDept(int pagenum)
{
  switch(pagenum)
  {
    case 1:
    tft.fillRect(0,50,128,110,BLACK);
    tft.setCursor(30,70);
    tft.setFont();
    tft.setTextSize(1);
    tft.setTextColor(RED);
    tft.println("ENGINEERING");
    tft.println();
    tft.setTextColor(WHITE);
    tft.println("  1. ETE     5. CIV");
    tft.println();
    tft.println("  2. CSE     6. FAE");
    tft.println();
    tft.println("  3. EEE");
    tft.println();
    tft.println("  4. ACCE");
    tft.println();
    break;

    case 2:
    tft.fillRect(0,50,128,110,BLACK);
    tft.setCursor(30,70);
    tft.setFont();
    tft.setTextSize(1);
    tft.setTextColor(RED);
    tft.println("ARCITECTURE");
    tft.println();
    tft.setTextColor(WHITE);
    tft.println("  1. ART");

    break;

    case 3:
    tft.fillRect(0,50,128,110,BLACK);
    tft.setCursor(40,70);
    tft.setFont();
    tft.setTextSize(1);
    tft.setTextColor(RED);
    tft.println("SCIENCE");
    tft.println();
    tft.setTextColor(WHITE);
    tft.println("  1. MAT     5. ESD");
    tft.println();
    tft.println("  2. STA");
    tft.println();
    tft.println("  3. CHE");
    tft.println();
    tft.println("  4. PHY");
    tft.println();
    break;

    case 4:
    tft.fillRect(0,50,128,110,BLACK);
    tft.setCursor(30,70);
    tft.setFont();
    tft.setTextSize(1);
    tft.setTextColor(RED);
    tft.println("LIFE SCIENCE");
    tft.println();
    tft.setTextColor(WHITE);
    tft.println("  1. PHR     5. BOT");
    tft.println();
    tft.println("  2. BGE");
    tft.println();
    tft.println("  3. BMB");
    tft.println();
    tft.println("  4. PSY");
    tft.println();
    break;

    case 5:
    tft.fillRect(0,50,128,110,BLACK);
    tft.setCursor(40,70);
    tft.setFont();
    tft.setTextSize(1);
    tft.setTextColor(RED);
    tft.println("HUMANITIES");
    tft.println();
    tft.setTextColor(WHITE);
    tft.println("  1. BAN");
    tft.println();
    tft.println("  2. ENG");
    break;

    case 6:
    tft.fillRect(0,50,128,110,BLACK);
    tft.setCursor(25,70);
    tft.setFont();
    tft.setTextSize(1);
    tft.setTextColor(RED);
    tft.println("SOCAL SCIENCE");
    tft.println();
    tft.setTextColor(WHITE);
    tft.println("  1. SOC");
    tft.println();
    tft.println("  2. IR");
    tft.println();
    tft.println("  3. ECO");
    tft.println();
    tft.println("  4. POS");
    tft.println();
    break;

    case 7:
    tft.fillRect(0,50,128,110,BLACK);
    tft.setCursor(40,70);
    tft.setFont();
    tft.setTextSize(1);
    tft.setTextColor(RED);
    tft.println("BISNUSS");
    tft.println();
    tft.setTextColor(WHITE);
    tft.println("  1. MAG     5. THM");
    tft.println();
    tft.println("  2. AIS");
    tft.println();
    tft.println("  3. MKT");
    tft.println();
    tft.println("  4. FAB");
    tft.println();
    break;

    case 8:
    tft.fillRect(0,50,128,110,BLACK);
    tft.setCursor(40,70);
    tft.setFont();
    tft.setTextSize(1);
    tft.setTextColor(RED);
    tft.println("LAW & AGRI");
    tft.println();
    tft.setTextColor(WHITE);
    tft.println("  1. LAW");
    tft.println();
    tft.println("  2. AGRI");
    break;

    
  }
  
}



void printTheTeam(String teamNumberone, String teamNumbertwo)
{
  tft.fillRect(45,0,30,20,BLACK);
  tft.setCursor(50,10);
  tft.print(teamNumberone);

   tft.fillRect(90,0,30,20,BLACK);
  tft.setCursor(95,10);
  tft.print(teamNumbertwo);
}


