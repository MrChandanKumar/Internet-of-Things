/*
  -------------------BSMRSTU Sports Server---------------------------------
  ............Programmer : Chandan Kumar Mondal............................
  .....................ID : 20151102053....................................
  .........................ETE,BSMRSTU.....................................
  -------------------------version:2.3-------------------------------------
  =============29.09.2019=====================3.51AM=======================
*/


// Library Section..............................................
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <Fonts/chandan_special_font_15.h> //Rochester_Regular_30
#include <Fonts/chandan_font_four.h> //URW_Chancery_L_Medium_Italic_20
//----------------------------------------------------------------

// Object Section...............................................
DHT ck(D3, DHT22);
ESP8266WebServer server(80);
Adafruit_ST7735 tft = Adafruit_ST7735(D2,  D4, D1);
//----------------------------------------------------------------

// Variable declaration.........................................
#define SSID "ETEian"
#define PASS "20151102053"
int led = D0;
int green = D6;
int blue = D8;
String firstteam = ""; //Name of the First team in Cricket, football and Badminton
String Secondteam = ""; //Name of the Second team in Cricket, football and Badminton
int teamA = 0; //Score for first team in Badminton and Football
int teamB = 0; //Score for second team in Badminton and Football
int setTeamA=0; //The SET value for First team in Badminton
int setTeamB = 0; //The SET value for Second team in Badminton
int runsTeamA = 0; //Runs of Fist team in Cricket
int runsTeamB = 0; //Runs of Second team in Cricket
int wicketTeamA = 0; //Wicket for First team
int wicketTeamB = 0; //Wicktet for Second team
//------------Selection---------------
int first_team = 10; 
int Second_team = 11;
//-------End of the Selection--------
int currentTeam = first_team;
int last_press = 0;
int overs_number = 0; //Over number for first team
int balls_number = 0; //Ball number for the Second team
int overs_for_second_team = 0; //Over number for first team
int ball_for_second_team = 0; //Ball number for the second team
int full_over = 0; //Full over number
int terget = 0; //Terget runs
int final_set=0; //Full SET numbers
int first_bat = 0; //Selection

//Password Section
int Password_length = 0;
String Main_password = "";

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
boolean first_team_done = false;
boolean firststep = false;
boolean secondstep = false;
boolean final_step = false;
boolean matrix_mode = false;
boolean gameover = false;
//------------------------------------------------------------------
/*
    Memory Segmentation
    
    Segment         |        value
    ---------------------------------
      0                   Game Status
    ---------------------------------
      10                  TeamA
      11                  TeamB
    ---------------------------------
      13                  RunsTeamA
      14                  WicketTeamA
      15                  Ball_teamA
      16                  Overs_number
    ---------------------------------
      17                  RunsTeamB
      18                  WicketTeamB
      19                  Ball_for_Second_team
      20                  Over_for_Second_team
    ---------------------------------
      21                  Set_TeamA
      22                  Set_TeamB
    ---------------------------------
      30                  GameMode (1-Cricket, 2-Football, 3-Badminton)
    ---------------------------------
      31                  First_team_name
      32                  Second_team_name
    ---------------------------------
      33                  Bat_first
    ---------------------------------
      34                  Full_Over
      35                  Full_set
    ---------------------------------
      40                  Current_team
      41                  Fist_team_done
      42                  First_bat
    ---------------------------------
      48                  Length of the Password 
      49                  Password status... 0 for clear and 1 for set
      50-...              Password


*/



//Message Handle Section.........................................
void handle_msg() {
  server.send(200, "text/html", myform());
  String msg = server.arg("msg");
  String non_orginal = msg;

  non_orginal.toLowerCase();
  if (non_orginal == "eteian speaking") {
    firststep = true;
  }
  if (non_orginal == "chandan") {
    if (firststep) {
      secondstep = true;
    }
  }
  if (non_orginal == "20151102053")
  {
    if (firststep && secondstep)
    {
      final_step = true;
    }
  }

  if (final_step)
  {
    if (non_orginal == "led on") {
      digitalWrite(led, HIGH);
    }
    else if (non_orginal == "led off") {
      digitalWrite(led, LOW);
      msg = "";
    }
    else if (non_orginal == "end") {
      firststep = false;
      secondstep = false;
      final_step = false;
      msg = "";
    }
  }

  if (non_orginal == "matrix on") {
    matrix_mode = true;
  }
  if (non_orginal == "matrix off")
  {
    matrix_mode = false;
  }

  if (matrix_mode)
  {
    msg += '\n';
    char carrier[50];
    msg.toCharArray(carrier, 50);
    Serial.write(carrier);
  }
}
//---------------------------------------------------------------------------------------------------------



void setup(void) {
  Serial.begin(9600);
  EEPROM.begin(200);
  
  ck.begin();
  //clear_the_memory();
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
      mycursor = 70;
    }
  }
  //-------------------------------------------------
  IPAddress ip(192,168,137,167);              //|
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
  tft.setFont(&URW_Chancery_L_Medium_Italic_20);
  tft.setCursor(15, 60);
  tft.setTextColor(GREEN);
  tft.println("CONNECT");
  tft.println();
  tft.setFont();
  tft.print(" to ");
  tft.print(SSID);
  delay(2000);
  Password_length = EEPROM.read(48);
  if(EEPROM.read(49)==1)
  {
    Password_box();
  }  
  tft.fillScreen(BLACK);

  if(EEPROM.read(0)==1)
  {
      GoToBack();
  }
  else if(EEPROM.read(0)==0)
  {
      gameConfigure();
  }
}

char upcoming;

void loop(void) {
//handle my Client
  server.handleClient();
//Take care of Cicuit Operation
  if (Serial.available() > 0)
  {
    upcoming = Serial.read();
//------------------------If Pressed ONE(1)----------------------------------------------------------------------
    if (upcoming == '1')
    {
      if (football || badminton)
      {
        teamA += 1;
        last_press = 1;
      if(badminton)
      {
        if(teamA>final_set)
          {
            setTeamA++;
            teamA=0;
            teamB=0;
          }
       }   
      }
      if (cricket)
      {
        currentTeam == first_team ? runsTeamA += 1 : runsTeamB += 1;
      }
    }

//------------------------If Pressed TWO(2)----------------------------------------------------------------
    else if (upcoming == '2')
    {
      if (cricket)
      {
        currentTeam == first_team ? runsTeamA += 2 : runsTeamB += 2;
      }
    }
//------------------------If Pressed THREE(3)-------------------------------------------------------------
    else if (upcoming == '3')
    {
      if (football || badminton)
      {
        teamB += 1;
        last_press = 2;
        if(badminton)
        {
          if(teamB>final_set)
            {
              teamB=0;
              teamA=0;
              setTeamB++;
            }
        }
        
      }
      if (cricket)
      {
        currentTeam == first_team ? runsTeamA += 3 : runsTeamB += 3;
      }
    }
//------------------------If pressed FOUR(4)------------------------------------------------------------
    else if (upcoming == '4')
    {
      if (cricket)
      {
        currentTeam == first_team ? runsTeamA += 4 : runsTeamB += 4;
      }
    }
//------------------------If Pressed FIVE(5)-------------------------------------------------------------
    else if (upcoming == '5')
    {
      if (cricket)
      {
        currentTeam == first_team ? runsTeamA += 5 : runsTeamB += 5;
      }
    }
//------------------------If Pressed SIX(6)--------------------------------------------------------------
    else if (upcoming == '6')
    {
      if (cricket)
      {
        currentTeam == first_team ? runsTeamA += 6 : runsTeamB += 6;
      }
    }
//------------------------If pressed SEVEN(7)--------------------------------------------------------------------------
    else if (upcoming == '7')
    {
      if (football || badminton)
      {
        teamA -= 1;
        last_press = 1;

        if(teamA<0)
        {
          teamA=0;
          setTeamA--;
          if(badminton)
          {
            if(setTeamA<0)
              {
                setTeamA=0;
              }
          }
          
        }
      }

      if (cricket)
      {
        currentTeam == first_team ? runsTeamA -= 1 : runsTeamB -= 1;
        runsTeamA < 0 ? runsTeamA = 0 : runsTeamA = runsTeamA;
        runsTeamB < 0 ? runsTeamB = 0 : runsTeamB = runsTeamB;
      }
    }
//------------------------If Pressed EIGHT(8)------------------------------------------------------------
    if (upcoming == '8')
    {
      if (cricket)
      {
        if (currentTeam == first_team)
        {
          balls_number++;
          if (balls_number > 5)
          {
            balls_number = 0;
            overs_number++;
            if (overs_number > full_over)
            {
              overs_number = full_over;
            }
          }
        }
        else if (currentTeam == Second_team)
        {
          ball_for_second_team++;
          if (ball_for_second_team > 5)
          {
            ball_for_second_team = 0;
            overs_for_second_team++;
            if (overs_for_second_team > full_over)
            {
              overs_for_second_team = full_over;
            }
          }
        }
      }
    }
//------------------------If Pressed NINE(9)-----------------------------------------------
    else if (upcoming == '9')
    {
      if (football || badminton)
      {
        teamB -= 1;
        last_press = 2;
        if(teamB<0)
        {
          teamB=0;
          if(badminton)
          {
            setTeamB--;
          if(setTeamB<0)
              {
                setTeamB=0;
              }
          }
          
        }
      }

      if (cricket)
      {
        currentTeam == first_team ? balls_number-- : ball_for_second_team--;
        if (ball_for_second_team < 0)
        {
          ball_for_second_team = 5;
          overs_for_second_team--;
          if (overs_for_second_team < 0)
          {
            overs_for_second_team = 0;
            ball_for_second_team = 0;
          }
        }

        if (balls_number < 0)
        {
          balls_number = 5;
          overs_number--;
          if (overs_number < 0)
          {
            overs_number = 0;
            balls_number=0;
          }
        }
      }
    }
//------------------------If Pressed ZERO(0)--------------------------------------------------
    else if (upcoming == '0')
    {
      if (cricket)
      {
        currentTeam == first_team ? currentTeam = Second_team : currentTeam = first_team;
        currentTeam == first_team ? EEPROM.write(40,1) : EEPROM.write(40,0);
        EEPROM.commit();
        first_team_done == true? first_team_done=false : first_team_done=true;
      }
    }
//------------------------If Pressed #--------------------------------------------------------
    else if (upcoming == '#')
    {
      first_team_done == true ? first_team_done = false : first_team_done = true;
      first_team_done == true ? EEPROM.write(41,0) : EEPROM.write(41,1);
      EEPROM.commit();
      currentTeam == first_team ? currentTeam = Second_team : currentTeam = first_team;
    }

//------------------------If Pressed A--------------------------------------------------------------
  else if (upcoming == 'A')
  {
    if (cricket)
    {
      currentTeam == first_team ? wicketTeamA += 1 : wicketTeamB += 1;
    }
  }
//------------------------If Pressed B-------------------------------------------------------------
  else if (upcoming == 'B')
  {
    if (cricket)
    {
      currentTeam == first_team ? wicketTeamA -= 1 : wicketTeamB -= 1;
      wicketTeamA < 0 ? wicketTeamA = 0 : wicketTeamA = wicketTeamA;
      wicketTeamB < 0 ? wicketTeamB = 0 : wicketTeamB = wicketTeamB;
    }
  }
//------------------------If Pressed C-------------------------------------------------------------
  else if (upcoming == 'C')
  {
    tft.fillScreen(BLACK);
    tft.setCursor(5, 40);
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
          overs_number=0;
          overs_for_second_team=0;
          ball_for_second_team=0;
          first_team_done = false;
          balls_number=0;
          gameover=false;
          full_over=0;
          terget=0;
          setTeamA=0;
          setTeamB=0;
          final_set=0;
          tft.fillScreen(BLACK);
          clear_the_memory();
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
//------------------------If Pressed #---------------------------------------------------------

//------------------------If Pressed D----------------------------------------------------------
  else if (upcoming == 'D')
    {
      Confirmation();
    }
  ScoreBoard();
 }
}

String myform() {
  String form =
    "<!DOCTYPE html>"
    "<html>"
    "<head>";
  if (!final_step) {
    form += "<meta http-equiv=\"refresh\" content=\"10\" >" ;
  }
  form += "<style>"
          "body{ background: rgb(127, 111, 221);}"
  ".heading{  background: linear-gradient(45deg,rgb(199, 56, 56),rgb(111, 250, 122),rgba(102, 55, 231, 0.233),rgb(71, 39, 214),rgb(199, 56, 56));  text-align: center;  padding: 10px;  color:rgb(37, 37, 35);  border: 2px solid black;  line-height: 1px;  font-family:  Cooper Std Black;  letter-spacing: 10px;  border-radius: 20px 20px 20px 20px;  background-size:400%;  transition: 0.5s ease; }"
  " .heading:hover{border-left: 2px dotted black; border-bottom: 2px dashed black; font-family:Times New Roman; letter-spacing: 20px; color: white;  animation: animate 8s linear infinite; }"
  " @keyframes animate { 0%{  background-position: 0%;  } 50%{  background-position: 200%; }100%{  background-position: 400%;  }  }"
  " p{position: absolute;  text-transform: uppercase;  top:50%;  left: 50%;  color:white;  transform: translate(-50%,-50%);  height: 50px;  width:200px;  text-align: center;  font-family: 'Times New Roman', Times, serif;  font-size: 20px;  line-height: 50px; box-sizing: border-box;  background: linear-gradient(30deg,#db2424,#165486,#d6158c);  border-radius: 30px;  background-size: 400%;}"
  "P:hover{ animation: animate 5s linear infinite;}"
  "@keyframes animate{ 0%{  background-position: 0%; } 100%{  background-position: 400%;  }}"
  "p::before{ content: '';  position: absolute;  top:-5; bottom: -5; left: -5;  right: -5; z-index: -1; background: linear-gradient(30deg,#db2424,#165486,#d6158c); background-position: 400%;border-radius: 40px; opacity: 0; transition: 0.5s ease;}"
  " p:hover::before{filter: blur(20px);opacity: 1;animation: animate 5s linear infinite;}"
  ".myteam{ width:20px;color:white; background: black;font-family: 'Times New Roman', Times, serif;letter-spacing: 5px;padding: 10px;font-size: 30px;padding: 10px;}"
  ".myruns{ background: white; font-family: Blackoak std; font-size: 30px;line-height: -11px; padding: 10px;}"
  ".mytable{background: black;border: 3px dotted white; margin-left: 380px; margin-top: 180px;}"
  ".myover{background: #abcdef;color: black;font-family: 'Franklin Gothic Medium', 'Arial Narrow', Arial, sans-serif;font-size: 30px; padding: 10px;}"
  ".mytarget{ background: #fffeee;font-family: 'Franklin Gothic Medium', 'Arial Narrow', Arial, sans-serif;color:black; font-size: 30px; padding: 10px;}"
  ".myoppo{  background: black;  color: white; font-family: 'Times New Roman', Times, serif; letter-spacing: 5px;padding: 10px;font-size: 30px; }"
  " .ter{color: white;font-family: 'Franklin Gothic Medium', 'Arial Narrow', Arial, sans-serif;text-align: center; }"
  " a{ position: absolute; top: 200px;left:305px;height: 40px;width: 50%;text-transform: uppercase;font-family:'Times New Roman', Times, serif; padding-bottom: 10px;font-size: 40px; background: green; border: 2px dotted black;text-align: center;border-radius: 30px; letter-spacing: 5px; color: white;transition: 0.5s ease;}"
  "a:hover{background: white;color:black; letter-spacing: 10px;}"
  " .myscore{background: white;font-family: 'Franklin Gothic Medium', 'Arial Narrow', Arial, sans-serif;font-size: 50px; padding: 10px; text-align: center;}"
  ".vs{background:linear-gradient(30deg,rgb(189, 132, 132),rgb(13, 230, 13),blue); color:black;font-family: 'Times New Roman', Times, serif; padding: 10px;font-size: 50px;}"
  ".footform{background: black;border: 3px dotted white;margin-left: 440px;margin-top: 180px;}"
  ".badform{background: black;border: 3px dotted white; margin-left: 420px; margin-top: 180px;}"
  ".myset{background: #165486;color: black;font-family: 'Franklin Gothic Medium', 'Arial Narrow', Arial, sans-serif;font-size: 30px;border: 2px solid green;}"
  "footer{position: absolute;bottom: 10px;width: 100%;text-align: center;background: black;font-family: 'Franklin Gothic Medium', 'Arial Narrow', Arial, sans-serif;font-size: 20px;color:white;border: 2px solid black; opacity: 0.3;transition: 0.5s ease;}"
  " footer:hover{opacity: 1;word-spacing: 5px;}"
  ".myfield{position: absolute; top:80%;left:70%;background: rgb(11, 26, 238);color: black;border: 3px dashed BLACK;;font-family: 'Times New Roman', Times, serif;padding:10px;}"
  " .mytext{background: white;color:black;border:2px dotted black;border-radius: 10px;}"
  ".mybtn{background: black;color:white;text-transform: uppercase;letter-spacing: 3px;border-radius: 10px;transition: 0.5s ease;margin-left: 5px;}"
  ".mybtn:hover{ background: white;color:black;}"
  " .details{position: absolute;bottom: 50px;height: 150px;width: 300px;background: pink;border: 2px solid green; outline: 2px dotted black;border-radius: 10px;}"
  ".un{position: absolute;text-decoration: none;text-align: center;margin: 5px;font-family: Cambria, Cochin, Georgia, Times, Times New Roman, serif;font-weight: bold;}"
  "h2{margin-top: 0px;text-transform: uppercase;letter-spacing: 5px;text-align: center;background: red;border: 2px solid black;}"
  ".team-name{position: absolute;left:350px;bottom: 50px;background: white;border: 3px solid black;font-family: 'Franklin Gothic Medium', 'Arial Narrow', Arial, sans-serif;}"
  " </style>"
  " </head>"
  " <body>"
  "<div class=\"heading\"><h1>BSMRSTU Sports</h1></div>";

  if (cricket)
  {
    form += "<a>Cricket</a>"

            "<table border=\"1\" class=\"mytable\">"
            "<tr>"
            "<td class=\"myteam\">";
              currentTeam == first_team ? form += firstteam : form += Secondteam;
              form += "</td> <td class=\"myruns\" >";
              String full_score = "";
              if(currentTeam==first_team)
              {
                full_score = String(runsTeamA);
                if(wicketTeamA<10){full_score+="-"+String(wicketTeamA);}
              }else{
                full_score= String(runsTeamB);
                if(wicketTeamB<10){full_score+= "-"+String(wicketTeamB);}
              }
              form += full_score;
              form += "</td> <td class=\"myover\">";
              String full_over_string ="";
              currentTeam == first_team ? full_over_string = String(overs_number) + "." + String(balls_number) : full_over_string = String(overs_for_second_team) + "." + String(ball_for_second_team);
              full_over_string += "(" + String(full_over) + ")";
              form += full_over_string;
              form += "</td> <td class=\"mytarget\">";
              currentTeam == first_team ? form += runsTeamB : form += runsTeamA;
              form += "</td> <td class=\"myoppo\">";
              currentTeam == Second_team ? form += firstteam : form += Secondteam;
    form += "</td>"
            "</tr>";
if(!gameover)
{
            form+="<tr>"
            "<td colspan=\"5\" class=\"ter\"><marquee>";
            if(first_team_done)
            {
              String notification = "";
              currentTeam == Second_team ?notification= Secondteam + " needs " + String((runsTeamA+1) - runsTeamB) + " to win from " + String((full_over * 6) - ((overs_for_second_team * 6) + ball_for_second_team)) : notification= firstteam + " needs " + String((runsTeamB+1) - runsTeamA) + " to win from " + String((full_over * 6) - ((overs_number * 6) + overs_for_second_team));
              notification += "balls";
              form += notification;
            }else{
              form+="";  //It's keep empty for the next version......
                        //In the next version.. I shall added the run rate .. required rate and current rate
            }
              
    form += "</marquee></td>"
            "</tr>";
}
     form+=       "</table>";
  }

  if(football)
  {
    form+="<a>Football</a>"
        "<table border=\"1\" class=\"footform\">"
            "<tr>"
                "<td class=\"myteam\">";
                form+=firstteam;
                form+="</td> <td class=\"myscore\">";
                form+=String(teamA);
                form+="</td> <td class=\"vs\">vs</td> <td class=\"myscore\">";
                form+=String(teamB);
                form+="</td> <td class=\"myoppo\">";
                form+=Secondteam;
                form+="</td>"
            "</tr>"
        "</table>";
  }

  if(badminton)
  {
    form+="<a>Badminton</a>"
        "<table border=\"1\" class=\"badform\">"
            "<tr>"
               "<td class=\"myteam\">";
               form+=firstteam;
               form+="</td> <td class=\"myscore\">";
               form+=String(teamA);
               form+="</td><td class=\"myset\">";
               form+=String(setTeamA);
               form+="</td> <td class=\"vs\">vs</td> <td class=\"myset\">";
               form+=String(setTeamB);
               form+="</td> <td class=\"myscore\">";
               form+=String(teamB);
               form+="</td><td class=\"myoppo\">";
               form+=String(Secondteam);
               form+="</td>"
            "</tr>"
        "</table>";
  }
 if(gameover)
 {
 form+= "<div class=\"details\">"
            "<h2> Results </h2>"
          "<u class=\"un\">";
          String winner = "";
          String looser = "";
          String final_message="";
          int bigscore=0;
          int lowscore=0;

          if(football)
          {
            if(teamA!=teamB)
            {
              if(teamA>teamB){
                  winner=firstteam;
                  looser=Secondteam;
                  bigscore=teamA;
                  lowscore=teamB;
                }else{
                  winner=Secondteam;
                  looser=first_team;
                  bigscore=teamB;
                  lowscore=teamA;
                }
                final_message=give_the_full_team_name(winner) + " beat "+ give_the_full_team_name(looser) + " by " + String(bigscore)+"-"+String(lowscore)+" goal";
                form+=final_message;
            }else{
              form+="Match Draw";
            }     
          }

          else if(badminton)
          {
            if(setTeamA!=setTeamB)
            {
              if(setTeamA>setTeamB)
              {
                winner=firstteam;
                looser=Secondteam;
                bigscore=setTeamA;
                lowscore=setTeamB;
              }else{
                winner=Secondteam;
                looser=firstteam;
                bigscore=setTeamB;
                lowscore=setTeamA;
              }
              final_message=give_the_full_team_name(winner)+" beats "+give_the_full_team_name(looser)+" by "+String(bigscore)+" / "+ String(lowscore)+" sets";
              form+=final_message;
            }else{
              form+="Match Draw";
            }
          }
          else if(cricket){
            if(first_bat == first_team && runsTeamA<runsTeamB)
            {
              form+= Secondteam + " won by " + String(10-wicketTeamB)+ " wickets";
            }
            if(first_bat == first_team && runsTeamA>runsTeamB)
            {
              form+= firstteam+" won by " + String(runsTeamA-runsTeamB) + " runs";
            }
            if(first_bat== Second_team && runsTeamA<runsTeamB)
            {
              form+= Secondteam + " won by " + String(runsTeamB-runsTeamA)+ " runs";
            }
            if(first_bat== Second_team && runsTeamA>runsTeamB)
            {
              form+= firstteam + " won by " + String(10-wicketTeamA) + " wickets";
            }
            if(runsTeamB==runsTeamA)
            {
              form+="Match Draw";
            }
          }

          
          form+="</u>"
        "</div>";
 }else{
  form+="<div class=\"team-name\">"
            "<marquee>";
            String printable_text = give_the_full_team_name(firstteam)+" vs "+ give_the_full_team_name(Secondteam);
            form+=printable_text;
            form+="</marquee>"
        "</div>";
 }
      
   form+= "<form action=\"msg\" class=\"myfield\">"
          "<b>MESSAGE : </b><input type=\"text\" class=\"mytext\" name=\"msg\" size: 50px autofocus><input type=\"submit\" class=\"mybtn\" value=\"Submit\">"
        "</form>"
        "<footer>Developed by : Chandan Kumar Mondal (ETE,BSMRSTU,20151102053)</footer>"
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
  tft.setCursor(10, 20);
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
  tft.println();
  tft.println("  D - Settings");

  boolean GAME = true;
  char cmd;
  while (GAME) {
    if (Serial.available() > 0)
    {
      cmd = Serial.read();

      if(cmd=='D')
      {
          tft.fillScreen(BLACK);
          tft.setCursor(10, 20);
          tft.setFont();
          tft.setTextColor(RED);
          tft.setTextSize(1);
          tft.println("SETTINGS");
          tft.println();
          tft.setTextColor(GREEN);
          tft.println("  A - Set Password");
          tft.println();
          tft.println("  B - Remove");
          tft.println();
          tft.println("  C - Change Password ");

          boolean wait_here = true;
          char command_me;
          while(wait_here)
          {
            if(Serial.available()>0)
            {
              command_me=Serial.read();

              if(command_me=='A')
              {
                if(EEPROM.read(49)==0)
                {
                  Set_the_password();
                }else{
                  tft.fillScreen(BLACK);
                  tft.setCursor(10, 20);
                  tft.setFont();
                  tft.setTextSize(1);
                  tft.setTextColor(GREEN);
                  tft.println(" Password Active ");
                  delay(3000);
                  gameConfigure();
                }  
              }
              if(command_me=='B')
              {
                if(EEPROM.read(49)==1)
                {
                  Remove_password();
                }else{
                  tft.fillScreen(BLACK);
                  tft.setCursor(10, 20);
                  tft.setFont();
                  tft.setTextSize(1);
                  tft.setTextColor(GREEN);
                  tft.println(" No password working ");
                  delay(3000);
                  gameConfigure();
                } 
                
              }
              if(command_me=='C')
              {
                if(EEPROM.read(49)==1)
                {
                  Change_the_password();
                }else{
                  tft.fillScreen(BLACK);
                  tft.setCursor(10, 20);
                  tft.setFont();
                  tft.setTextSize(1);
                  tft.setTextColor(GREEN);
                  tft.println(" No password working ");
                  delay(3000);
                  gameConfigure();
                }                
              }
            }
          }
      }

      if (cmd == 'C')
      {
        tft.fillScreen(BLACK);
        tft.setCursor(5, 40);
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
//**************************************************EEPROM**********************************
                EEPROM.write(30,1);
//******************************************************************************************
              teamSelection();
            }
          }
        }
      }
      else if (cmd == 'A')
      {
        tft.fillScreen(BLACK);
        tft.setCursor(5, 40);
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
//**************************************************EEPROM**********************************
                EEPROM.write(30,2);
//******************************************************************************************
              teamSelection();
            }
          }
        }
      }
      else if (cmd == 'B')
      {
        tft.fillScreen(BLACK);
        tft.setCursor(5, 40);
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
//**************************************************EEPROM**********************************
                EEPROM.write(30,3);
//******************************************************************************************
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
  tft.setCursor(10, 20);
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
          first_team_done == false ? EEPROM.write(31,1) : EEPROM.write(32,1);
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
          EEPROM.commit();
        }
        else if (num == 2)
        {
          first_team_done == false ? firstteam = "ART" : Secondteam = "ART";
          first_team_done == false ? EEPROM.write(31,7) : EEPROM.write(32,7);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 3)
        {
          first_team_done == false ? firstteam = "MAT" : Secondteam = "MAT";
          first_team_done == false ? EEPROM.write(31,10) : EEPROM.write(32,10);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 4)
        {
          first_team_done == false ? firstteam = "PHR" : Secondteam = "PHR";
          first_team_done == false ? EEPROM.write(31,12) : EEPROM.write(32,12);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 5)
        {
          first_team_done == false ? firstteam = "BAN" : Secondteam = "BAN";
          first_team_done == false ? EEPROM.write(31,15) : EEPROM.write(32,15);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 6)
        {
          first_team_done == false ? firstteam = "SOC" : Secondteam = "SOC";
          first_team_done == false ? EEPROM.write(31,24) : EEPROM.write(32,24);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 7)
        {
          first_team_done == false ? firstteam = "MAG" : Secondteam = "MAG";
          first_team_done == false ? EEPROM.write(31,21) : EEPROM.write(32,21);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 8)
        {
          first_team_done == false ? firstteam = "LAW" : Secondteam = "LAW";
          first_team_done == false ? EEPROM.write(31,22) : EEPROM.write(32,22);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }

      }

      else if (command == '2')
      {
        if (num == 1)
        {
          first_team_done == false ? firstteam = "CSE" : Secondteam = "CSE";
          first_team_done == false ? EEPROM.write(31,2) : EEPROM.write(32,2);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 3)
        {
          first_team_done == false ? firstteam = "STA" : Secondteam = "STA";
          first_team_done == false ? EEPROM.write(31,13) : EEPROM.write(32,13);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 4)
        {
          first_team_done == false ? firstteam = "BGE" : Secondteam = "BGE";
          first_team_done == false ? EEPROM.write(31,27) : EEPROM.write(32,27);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 5)
        {
          first_team_done == false ? firstteam = "ENG" : Secondteam = "ENG";
          first_team_done == false ? EEPROM.write(31,16) : EEPROM.write(32,16);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 6)
        {
          first_team_done == false ? firstteam = "IR " : Secondteam = "IR ";
          first_team_done == false ? EEPROM.write(31,19) : EEPROM.write(32,19);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 7)
        {
          first_team_done == false ? firstteam = "AIS" : Secondteam = "AIS";
          first_team_done == false ? EEPROM.write(31,20) : EEPROM.write(32,20);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 8)
        {
          first_team_done == false ? firstteam = "AGR" : Secondteam = "AGR";
          first_team_done == false ? EEPROM.write(31,23) : EEPROM.write(32,23);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
      }
      else if (command == '3')
      {
        if (num == 1)
        {
          first_team_done == false ? firstteam = "EEE" : Secondteam = "EEE";
          first_team_done == false ? EEPROM.write(31,3) : EEPROM.write(32,3);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 3)
        {
          first_team_done == false ? firstteam = "CHE" : Secondteam = "CHE";
          first_team_done == false ? EEPROM.write(31,8) : EEPROM.write(32,8);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 4)
        {
          first_team_done == false ? firstteam = "BMB" : Secondteam = "BMB";
          first_team_done == false ? EEPROM.write(31,14) : EEPROM.write(32,14);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 6)
        {
          first_team_done == false ? firstteam = "ECO" : Secondteam = "ECO";
          first_team_done == false ? EEPROM.write(31,26) : EEPROM.write(32,26);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 7)
        {
          first_team_done == false ? firstteam = "MKT" : Secondteam = "MKT";
          first_team_done == false ? EEPROM.write(31,30) : EEPROM.write(32,30);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
      }
      else if (command == '4')
      {
        if (num == 1)
        {
          first_team_done == false ? firstteam = "ACCE" : Secondteam = "ACCE";
          first_team_done == false ? EEPROM.write(31,6) : EEPROM.write(32,6);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 3)
        {
          first_team_done == false ? firstteam = "PHY" : Secondteam = "PHY";
          first_team_done == false ? EEPROM.write(31,9) : EEPROM.write(32,9);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 4)
        {
          first_team_done == false ? firstteam = "PSY" : Secondteam = "PSY";
          first_team_done == false ? EEPROM.write(31,18) : EEPROM.write(32,18);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 6)
        {
          first_team_done == false ? firstteam = "POS" : Secondteam = "POS";
          first_team_done == false ? EEPROM.write(31,25) : EEPROM.write(32,25);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 7)
        {
          first_team_done == false ? firstteam = "FAB" : Secondteam = "FAB";
          first_team_done == false ? EEPROM.write(31,29) : EEPROM.write(32,29);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
      }
      else if (command == '5')
      {
        if (num == 1)
        {
          first_team_done == false ? firstteam = "CE " : Secondteam = "CE ";
          first_team_done == false ? EEPROM.write(31,4) : EEPROM.write(32,4);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 3)
        {
          first_team_done == false ? firstteam = "ESD" : Secondteam = "ESD";
          first_team_done == false ? EEPROM.write(31,11) : EEPROM.write(32,11);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 4)
        {
          first_team_done == false ? firstteam = "BOT" : Secondteam = "BOT";
          first_team_done == false ? EEPROM.write(31,17) : EEPROM.write(32,17);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
        else if (num == 7)
        {
          first_team_done == false ? firstteam = "THM" : Secondteam = "THM";
          first_team_done == false ? EEPROM.write(31,28) : EEPROM.write(32,28);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
      }
      else if (command == '6')
      {
        if (num == 1)
        {
          first_team_done == false ? firstteam = "FAE" : Secondteam = "FAE";
          first_team_done == false ? EEPROM.write(31,5) : EEPROM.write(32,5);
          EEPROM.commit();
          first_team_done = true;
          printTheTeam(firstteam, Secondteam);
        }
      }
      else if (command == '*')
      {
        if (cricket)
        {
          tft.fillScreen(BLACK);
          fixed_the_over();
          showTheC();
//**********************************************EEPROM***************************
          EEPROM.write(0,1);
          EEPROM.commit();
//*******************************************************************************
          wait = false;
        }
        if (football || badminton)
        {
          if (badminton)
          {
            fixed_the_set();
          }
          tft.fillScreen(BLACK);
          showTheFB();
//******************************************EEPROM******************************
          EEPROM.write(0,1);
          EEPROM.commit();
//******************************************************************************
          wait = false;
        }
        ScoreBoard();
        return;
      }
      else if (command == 'C')
      {
        first_team_done == true ? first_team_done = false : first_team_done = true;
        firstteam = "";
        printTheTeam(firstteam, Secondteam);
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
  tft.setCursor(60, 145);
  tft.print("IP: 192.168.137.20");

}

void showTheC()
{
  tft.fillScreen(BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 130);
  tft.println("1 to 6 Run.. A wicket up");;
  tft.println();
  tft.println("B wicket down.. 0-switch");
}

void showTheFB()
{
  tft.fillScreen(BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 130);
  tft.println("TeamA : 1 inc 7 dec");
  tft.println();
  tft.println("TeamB : 3 inc 9 dec");

}

void ScoreBoard()
{
  if (cricket)
  {
    if(currentTeam==first_team)
    {
      if(first_team_done)
      {
        if(runsTeamA>runsTeamB)
        {
          Confirmation();
        }
      }
      if(overs_number==full_over)
      {
        overs_number=full_over;
        balls_number = 0;
      }
    }else{
      if(first_team_done)
      {
        if(runsTeamA<runsTeamB)
        {
          Confirmation();
        }
      }
      if(overs_for_second_team==full_over)
      {
        Confirmation();
      }
    }
    tft.fillRect(0,0,122,115,BLACK);
    tft.setCursor(50, 10);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    currentTeam==first_team? tft.print(firstteam):tft.print(Secondteam);
    tft.print(" : ");
    tft.setTextColor(WHITE);
    tft.setCursor(10, 35);
    currentTeam==first_team? tft.print(runsTeamA):tft.print(runsTeamB);
    tft.print(" / ");
    currentTeam==first_team? tft.print(wicketTeamA):tft.print(wicketTeamB);

    tft.setCursor(50, 60);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    currentTeam==first_team ? tft.print(Secondteam):tft.print(firstteam);
    tft.print(" : ");
    tft.setCursor(10, 85);
    tft.setTextColor(WHITE);
    String showtheover;
    currentTeam==Second_team ? showtheover = String(overs_for_second_team) + "." + String(ball_for_second_team) : showtheover=String(overs_number) + "." + String(balls_number);
    tft.print(showtheover);
//*******************************************EEPROM*********************************************
    EEPROM.write(13,runsTeamA);
    EEPROM.write(14,wicketTeamA);
    EEPROM.write(15,balls_number);
    EEPROM.write(16,overs_number);
    EEPROM.write(17,runsTeamB);
    EEPROM.write(18,wicketTeamB);
    EEPROM.write(19,ball_for_second_team);
    EEPROM.write(20,overs_for_second_team);
    EEPROM.commit();
//**********************************************************************************************
  }
  else if (badminton || football)
  {
    tft.fillRect(40, 0, 80, 100, BLACK);
    tft.setCursor(5, 10);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    tft.print(firstteam);
    tft.print(" : ");
    tft.setTextColor(WHITE);
    tft.println (teamA);

    tft.setCursor(5, 50);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    tft.print(Secondteam);
    tft.print(" : ");
    tft.setTextColor(WHITE);
    tft.print(teamB);
//**************************************************EEPROM**************************************
    EEPROM.write(10,teamA);
    EEPROM.write(11,teamB);
    EEPROM.write(21,setTeamA);
    EEPROM.write(22,setTeamB);
    EEPROM.commit();
//**********************************************************************************************
  }
}

void printDept(int pagenum)
{
  switch (pagenum)
  {
    case 1:
      tft.fillRect(0, 50, 128, 110, BLACK);
      tft.setCursor(30, 70);
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
      tft.fillRect(0, 50, 128, 110, BLACK);
      tft.setCursor(30, 70);
      tft.setFont();
      tft.setTextSize(1);
      tft.setTextColor(RED);
      tft.println("ARCITECTURE");
      tft.println();
      tft.setTextColor(WHITE);
      tft.println("  1. ART");

      break;

    case 3:
      tft.fillRect(0, 50, 128, 110, BLACK);
      tft.setCursor(40, 70);
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
      tft.fillRect(0, 50, 128, 110, BLACK);
      tft.setCursor(30, 70);
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
      tft.fillRect(0, 50, 128, 110, BLACK);
      tft.setCursor(40, 70);
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
      tft.fillRect(0, 50, 128, 110, BLACK);
      tft.setCursor(25, 70);
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
      tft.fillRect(0, 50, 128, 110, BLACK);
      tft.setCursor(40, 70);
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
      tft.fillRect(0, 50, 128, 110, BLACK);
      tft.setCursor(40, 70);
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
  tft.fillRect(45, 0, 30, 20, BLACK);
  tft.setCursor(50, 10);
  tft.print(teamNumberone);

  tft.fillRect(90, 0, 30, 20, BLACK);
  tft.setCursor(95, 10);
  tft.print(teamNumbertwo);
}

void fixed_the_over()
{
  tft.setCursor(0,5);
  tft.setTextSize(1);
  tft.fillScreen(BLACK);
  tft.println("Enter the over : ");
  tft.println();
  tft.println();
  tft.setTextSize(2);
  char overs_command;
  String overs = "";
  boolean over_done = false;
  while (!over_done)
  {
    if (Serial.available() > 0)
    {
      overs_command = Serial.read();
      overs += overs_command;
      tft.setCursor(10,40);
      tft.print(overs);

      if (overs_command == '*')
      {
        overs.replace("*", "");
        full_over = constrain(overs.toInt(), 2, 50);
        EEPROM.write(34,full_over);
        EEPROM.commit();
        over_done = true;
        tft.setTextSize(1);
        tft.fillScreen(BLACK);
        fixed_the_bat();
      }

      if(overs_command=='C')
      {
        fixed_the_over();
      }
    }
  }
}

void fixed_the_set()
{
  tft.setCursor(0,5);
  tft.setTextSize(1);
  tft.fillScreen(BLACK);
  tft.println("Enter the Set : ");
  tft.println();
  tft.println();
  tft.setTextSize(2);
  String set = "";
  char set_command;
  boolean set_done = false;
  while (!set_done)
  {
    if (Serial.available() > 0)
    {
      set_command = Serial.read();
      set += set_command;
      tft.setCursor(10,70);
      tft.print(set);

      if (set_command == '*')
      {
        set.replace("*", "");
        final_set = constrain(set.toInt(), 1, 50);
        EEPROM.write(35,final_set);
        EEPROM.commit();
        set_done = true;
      }
      if(set_command=='C')
      {
        fixed_the_set();
      }
    }
  }
}

String give_the_full_team_name(String short_name)
{
  if(short_name=="ETE"){return "Electronics and Telecommunication Engineering";}
  else if(short_name=="CSE"){return "Computer Science and Engineering";}
  else if(short_name=="EEE"){return "Electrical and Electronics Engineering";}
  else if(short_name=="CE "){return "Civil Engineering";}
  else if(short_name=="FAE"){return "Food and Agro Engineering";}
  else if(short_name=="ACCE"){return "Applied Chemistry and Chemical Engineering";}
  else if(short_name=="ART"){return "Articture";}
  else if(short_name=="CHE"){return "Chemistry";}
  else if(short_name=="PHY"){return "Physics";}
  else if(short_name=="MAT"){return "Mathemitics";}
  else if(short_name=="ESD"){return "Enivironmatal Science and Digaster";}
  else if(short_name=="PHR"){return "Pharmacy";}
  else if(short_name=="STA"){return "Statictics";}
  else if(short_name=="BMB"){return "Biochemistry and Molicular Biology";}
  else if(short_name=="BAN"){return "Bangla";}
  else if(short_name=="ENG"){return "English";}
  else if(short_name=="BOT"){return "Botany";}
  else if(short_name=="PSY"){return "Phychology";}
  else if(short_name=="IR "){return "International Relationship";}
  else if(short_name=="AIS"){return "Accounting and Information System";}
  else if(short_name=="MAG"){return "Management";}
  else if(short_name=="LAW"){return "Law";}
  else if(short_name=="AGR"){return "Agriculture";}
  else if(short_name=="SOC"){return "Sociology";}
  else if(short_name=="POS"){return "Political Science";}
  else if(short_name=="ECO"){return "Economics";}
  else if(short_name=="BGE"){return "Biotechnology and Genetic Engineering";}
  else if(short_name=="THM"){return "Tourism and Hospitality Management";}
  else if(short_name=="FAB"){return "Fisharis";}
  else if(short_name=="MKT"){return "Marketing";}
}

void fixed_the_bat()
{
  boolean wait_here = true;
  tft.fillScreen(BLACK);
  tft.println("Bat...");
  tft.println();
  tft.setTextSize(2);
  tft.print("1. ");
  tft.println(firstteam);
  tft.print("2. ");
  tft.println(Secondteam);

  char upcoming_command;

  while(wait_here)
  {
    if(Serial.available())
    {
      upcoming_command=Serial.read();
      if(upcoming_command=='1')
      {
        currentTeam=first_team;
        EEPROM.write(33,1);
        EEPROM.commit();
        first_bat=first_team;
        wait_here=false;
        tft.fillScreen(BLACK);
        tft.setTextSize(1);
        EEPROM.write(42,0);
        EEPROM.commit();
      }
      else if(upcoming_command=='2')
      {
        currentTeam=Second_team;
        EEPROM.write(33,2);
        EEPROM.commit();
        first_bat = Second_team;
         wait_here=false;
        tft.fillScreen(BLACK);
        tft.setTextSize(1);
        EEPROM.write(42,1);
        EEPROM.commit();
      }
    }
  }
}

void Confirmation()
{
  tft.fillScreen(BLACK);
      tft.setTextSize(1);
      tft.println("Game End? Sure?");
      tft.println("D-sure");
      tft.println("C-cancel");
      char give_command;
      boolean hold_on = true;
      while(hold_on)
      {
        if(Serial.available()>0)
        {
          give_command=Serial.read();
          if(give_command=='C')
          {
            hold_on=false;
            gameover = false;
             if(cricket)
            {
              overs_for_second_team==full_over? overs_for_second_team--:overs_for_second_team=overs_for_second_team;
              overs_number == full_over ? overs_number--:overs_number=overs_number;
            }
            ScoreBoard();
          }

          else if(give_command=='D')
          {
            gameover==true? gameover=false:gameover=true;
            hold_on=false;
            tft.fillScreen(BLACK);
            tft.print("Done");
          }
        }
      }
}


void clear_the_memory()
{
    for(int i=0;i<45;i++)
    {
        EEPROM.write(i,0);
    }
     EEPROM.commit();
}

void GoToBack()
{
    runsTeamA = EEPROM.read(13);
    wicketTeamA = EEPROM.read(14);
    balls_number = EEPROM.read(15);
    overs_number = EEPROM.read(16);
    runsTeamB = EEPROM.read(17);
    wicketTeamB = EEPROM.read(18);
    ball_for_second_team = EEPROM.read(19);
    overs_for_second_team = EEPROM.read(20);
    setTeamA = EEPROM.read(21);
    setTeamB = EEPROM.read(22);
    teamA = EEPROM.read(10);
    teamB = EEPROM.read(11);
    full_over = EEPROM.read(34);
    final_set = EEPROM.read(35);
    EEPROM.read(40)==0 ? currentTeam=first_team: currentTeam==Second_team;
    EEPROM.read(41)==0 ? first_team_done=false:first_team_done==true;
    EEPROM.read(42)==0 ? first_bat=first_team:first_bat=Second_team;
    cricket = false;
    badminton = false;
    football = false;
    switch(EEPROM.read(30)){
        case 1:
        cricket = true;
        break;
        case 2:
        football = true;
        break;
        case 3:
        badminton = true;
        break;
    }
    EEPROM.commit();
    firstteam = ReadMe(EEPROM.read(31));
    Secondteam = ReadMe(EEPROM.read(32));
    ScoreBoard();
}

String ReadMe(int memory_val)
{
       if(memory_val==1){return "ETE";}
  else if(memory_val==2){return "CSE";}
  else if(memory_val==3){return "EEE";}
  else if(memory_val==4){return "CE ";}
  else if(memory_val==5){return "FAE";}
  else if(memory_val==6){return "ACCE";}
  else if(memory_val==7){return "ART";}
  else if(memory_val==8){return "CHE";}
  else if(memory_val==9){return "PHY";}
  else if(memory_val==10){return "MAT";}
  else if(memory_val==11){return "ESD";}
  else if(memory_val==12){return "PHR";}
  else if(memory_val==13){return "STA";}
  else if(memory_val==14){return "BMB";}
  else if(memory_val==15){return "BAN";}
  else if(memory_val==16){return "ENG";}
  else if(memory_val==17){return "BOT";}
  else if(memory_val==18){return "PSY";}
  else if(memory_val==19){return "IR ";}
  else if(memory_val==20){return "AIS";}
  else if(memory_val==21){return "MAG";}
  else if(memory_val==22){return "LAW";}
  else if(memory_val==23){return "AGR";}
  else if(memory_val==24){return "SOC";}
  else if(memory_val==25){return "POS";}
  else if(memory_val==26){return "ECO";}
  else if(memory_val==27){return "BGE";}
  else if(memory_val==28){return "THM";}
  else if(memory_val==29){return "FAB";}
  else if(memory_val==30){return "MKT";}
}

void Password_box()
{
  tft.fillScreen(BLACK);
  tft.setCursor(10,20);
  tft.setTextColor(GREEN);
  tft.setFont();
  tft.setTextSize(1);
  int count = 0;
  tft.println("Enter Password:");

  boolean mystatus = true;
  char upcoming_pass;
  String TrushPassword = "";
  tft.setTextColor(WHITE);
  

  while(mystatus)
  {
    if(Serial.available()>0)
    {
      upcoming_pass = Serial.read();
      TrushPassword+=upcoming_pass;
      tft.setCursor(count+=10,40);
      tft.print("*");
      
      if(upcoming_pass=='C')
      {
        TrushPassword="";
        tft.fillRect(0,30,120,30,BLACK);
        count=0;
      }
      if(upcoming_pass=='*')
      {
        TrushPassword.replace("*","");
        if(TrushPassword==Give_the_Main_password())
        {
          tft.fillScreen(BLACK);
          mystatus=false;
          break;
        }else{
          mystatus=false;
          Password_box();
        }
      }
    }
  }
}


void Set_the_password()
{
  tft.fillScreen(BLACK);
  tft.setCursor(10,20);
  tft.setTextColor(GREEN);
  tft.setFont();
  tft.setTextSize(1);
  int count = 0;
  tft.print("Birthday:");

  boolean mystatus = true;
  char upcoming_pass;
  String TrushPassword = "";
  tft.setTextColor(WHITE);
  
  while(mystatus)
  {
    if(Serial.available())
    {
      upcoming_pass = Serial.read();
      TrushPassword+=upcoming_pass;
      tft.setCursor(count+=10,40);
      tft.print(upcoming_pass);
      
      if(upcoming_pass=='C')
      {
        TrushPassword="";
        tft.fillRect(0,30,120,30,BLACK);
        count=0;
      }
      if(upcoming_pass=='*')
      {
        TrushPassword.replace("*","");
        if(TrushPassword=="17011995")
        {
          tft.fillScreen(BLACK);
          Done_give_password();
        }else{
          tft.fillScreen(BLACK);
          tft.setCursor(10,40);
          tft.setTextSize(2);
          tft.print("Wrong");
          delay(2000);
           Set_the_password();
        }
      }
    }
    if(upcoming_pass=='#')
    {
      mystatus=false;
      gameConfigure();
    }
  }
}

void Done_give_password()
{
  tft.fillScreen(BLACK);  //Barthdate is matched and ready to take password.
  tft.setCursor(10,20);
  tft.setTextColor(GREEN);
  tft.setFont();
  tft.setTextSize(1);
  int count = 0;
  tft.print("Password:");  // "   Password : "

  boolean mystatus = true;
  char upcoming_pass;
  String TrushPassword = "";
  tft.setTextColor(WHITE);

  while(mystatus)
  {
    if(Serial.available())   // take the password and store it to "TrushPassword"
    {
      upcoming_pass = Serial.read();
      TrushPassword+=upcoming_pass;
      tft.setCursor(count+=10,40);
      tft.print(upcoming_pass);  // Show the real char in the display
      
      if(upcoming_pass=='C')   // If press 'C' then given password will be deleted.
      {
        TrushPassword="";
        tft.fillRect(0,30,120,30,BLACK);
        count=0;
      }
      if(upcoming_pass=='*')  //If press '*', take the initial Password and ask for re-entering
      {
        tft.fillScreen(BLACK);
        tft.setCursor(10,20);
        tft.setTextColor(GREEN);
        tft.setFont();
        tft.setTextSize(1);
        int count = 0;
        tft.print("Re Type:");  // "    Re Type :  "
        count=0;
        String passone="";
        TrushPassword.replace("*","");  //Store the initial Password in the "passone"
        passone=TrushPassword;
        boolean re_enter = true;
        String second_pass = "";
        char second_command;
        tft.setTextColor(WHITE);
        while(re_enter)
        {
          if(Serial.available()>0)  // Take the second confirmation password and store in " second_pass"
          {
            second_command=Serial.read();
            second_pass += second_command;
            tft.setCursor(count+=10,40);
            tft.print(second_command);  // Show the real char in the display

            if(second_command=='C')   // If press 'C' then given password will be deleted.
            {
              second_pass="";
              tft.fillRect(0,30,120,30,BLACK);
              count=0;
            } 
            if(second_command=='*')  //Check the two passwords
            {
              second_pass.replace("*","");
              if(second_pass==passone) //If the password matched.. store in the memory
              {
                tft.fillScreen(BLACK);
                tft.setCursor(50,100);
                tft.print("Done");

                Password_length = passone.length(); // take the length of the passoword and store in 49
                EEPROM.write(48,Password_length);
                EEPROM.commit();

                for(int i=0;i<Password_length;i++)
                {
                  EEPROM.write(i+50,passone.charAt(i));
                  EEPROM.commit();
                }
                EEPROM.write(49,1); // Make a flag.. 
                EEPROM.commit(); // Done.. and back to the configure page.
                delay(1000);
                gameConfigure();
              }else{  //If passoword not Matched..... Ask again form the Done_give_passoword Function.
                tft.fillScreen(BLACK);
                tft.setCursor(50,100);
                tft.print("Not Matched");
                delay(1000);
                Done_give_password();
              }
            }                       
          }
        }
      }
    }
  }
}

void Remove_password()
{

  tft.fillScreen(BLACK);
  tft.setCursor(10,20);
  tft.setTextColor(GREEN);
  tft.setFont();
  tft.setTextSize(1);
  int count = 0;
  tft.print("Current:");

  boolean mystatus = true;
  char upcoming_pass;
  String TrushPassword = "";
  tft.setTextColor(WHITE);
  
  while(mystatus)
  {
    if(Serial.available())
    {
      upcoming_pass = Serial.read();
      TrushPassword+=upcoming_pass;
      tft.setCursor(count+=10,40);
      tft.print(upcoming_pass);
      
      if(upcoming_pass=='C')
      {
        TrushPassword="";
        tft.fillRect(0,30,120,30,BLACK);
        count=0;
      }
      if(upcoming_pass=='*')
      {
        TrushPassword.replace("*","");

        if(TrushPassword==Give_the_Main_password())
        {
          tft.fillScreen(BLACK);
          tft.setCursor(10,20);
          tft.setTextColor(GREEN);
          tft.setFont();
          tft.setTextSize(1);
          tft.print("Removed");
          EEPROM.write(49,0); // Turn off the flag
          EEPROM.commit();
          gameConfigure();
        }else{
          tft.fillScreen(BLACK);
          tft.setCursor(10,40);
          tft.setTextSize(2);
          tft.print("Wrong");
          delay(2000);
          gameConfigure();
        }
      }
    }
  }
}

void Change_the_password()
{
  tft.fillScreen(BLACK);
  tft.setCursor(10,20);
  tft.setTextColor(GREEN);
  tft.setFont();
  tft.setTextSize(1);
  int count = 0;
  tft.print("Current Password:");

  boolean mystatus = true;
  char upcoming_pass;
  String TrushPassword = "";
  tft.setTextColor(WHITE);
  
  while(mystatus)
  {
    if(Serial.available())
    {
      upcoming_pass = Serial.read();
      TrushPassword+=upcoming_pass;
      tft.setCursor(count+=10,40);
      tft.print(upcoming_pass);
      
      if(upcoming_pass=='C')
      {
        TrushPassword="";
        tft.fillRect(0,30,120,30,BLACK);
        count=0;
      }
      if(upcoming_pass=='*')
      {
        TrushPassword.replace("*","");

        if(TrushPassword==Give_the_Main_password())
        {
          Done_give_password();
        }else{
          tft.fillScreen(BLACK);
          tft.setCursor(10,40);
          tft.setTextSize(2);
          tft.print("Wrong");
          delay(2000);
          gameConfigure();
        }
      }
    }
  }
}

String Give_the_Main_password()
{
  String sendable_password = "";
  for(int i=0;i<Password_length;i++)
  {
    sendable_password += char(EEPROM.read(50+i)); 
  }
  EEPROM.commit();
  return sendable_password;
}


