// Library Section..............................................
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <LiquidCrystal.h>
//----------------------------------------------------------------

// Object Section...............................................
LiquidCrystal lcd(D3, D4, D5, D6, D7, D8);
DHT ck(D2, DHT22);
ESP8266WebServer server(80);

// Variable declaration.........................................
#define SSID "Chandan"
#define PASS "ssskac53"

int led = D0;
String firstteam = "";
String Secondteam = "";
int teamA =0;
int teamB = 0;
int runsTeamA =0;
int runsTeamB = 0;
int wicketTeamA = 0;
int wicketTeamB = 0;
int first_team = 10;
int Second_team = 11;
int currentTeam = first_team;
int last_press = 0;
//----------------------------------------------------------------

//------Boolean Varialbles---------------------------------------
boolean configure=false;
boolean cricket=false;
boolean football=false;
boolean badminton=false;
boolean team = false;


//------------------------------------------------------------------




//Messae Handle Section.........................................
void handle_msg() {
  server.send(200, "text/html", myform());
  String msg = server.arg("msg");

  msg.toLowerCase();
  if (msg == "clc")
  {
    clc();
  }
  else if (msg == "led on")
  {
    digitalWrite(led, HIGH);
  }
  else if (msg == "led off")
  {
    digitalWrite(led, LOW);
  }
  else {
    clc();
    lcd.setCursor(0, 0);
    lcd.print(msg);
  }
}
//---------------------------------------------------------------------------------------------------------



void setup(void) {
  Serial.begin(9600);
  lcd.begin(20, 4);
  ck.begin();
  pinMode(led, OUTPUT);
  int mycursor = 0;
  int warning = 0;
  lcd.setCursor(0, 0);
  lcd.print("Connecting");
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    lcd.setCursor(mycursor++, 1);
    lcd.print(".");


    if (mycursor > 19)
    {
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      mycursor = 0;
      warning++;

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

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected");
  lcd.setCursor(0, 1);
  lcd.print("192.168.137.20");
  delay(2000);
  lcd.clear();
  for (int i = 0; i < 20; i++)
  {
    lcd.setCursor(i, 2);
    lcd.print("_");
    delay(50);
  }
  lcd.setCursor(1, 3);
  lcd.print("IP: 192.168.137.20");
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
      lcd.clear();
      lcd.print("Your are entering,");
      lcd.setCursor(0, 1);
      lcd.print("Configure mode");
      lcd.setCursor(0, 2);
      lcd.print("will lost your data");
      lcd.setCursor(0, 3);
      lcd.print("* sure  C cancel");

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
            lcd.clear();
            gameConfigure();
          }
          else if (mycommand == 'C')
          {
            waiting = false;
            lcd.clear();
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
    "<head>"
    "<meta http-equiv=\"refresh\" content=\"15\" >"
    "<style>"

    "body{padding: 0; margin: 0;  background: pink}"
    "h1{background: black; transition: 0.5s all; color: white; text-align: center; border: 3px dotted white; border-radius: 30px; margin: 10px 20px;}"
    " h1:hover{ background: white; color: black; letter-spacing: 20px; border: 3px dotted black; }"
    ".display{ height: 100px; width: 200px; float: left; transition: 0.5s all ease; margin-left: 30px; background: #789654;; border: 3px solid black; border-radius: 30px; }"
    ".display:hover{border: 5px dotted black; background: #451691; }"
    ".humd{ height: 30px; width: 180px; transition: 0.5s ease; background : #bacd; position: absolute; margin: 10px 10px; text-align: center; font-size: 28px; }"
    ".humd:hover{ background: green; border: 2px solid black; text-transform: uppercase; overflow: hidden; border-radius: 10px; color: white; font-family: 'Times New Roman', Times, serif; }"
    ".temp{ height: 30px; width: 180px; background: #ccffff; position: absolute; margin-left: 10px; margin-top : 60px; text-align: center; font-size: 28px; }"
    ".temp:hover{ background: green; border: 2px solid black; text-transform: uppercase; overflow: hidden; border-radius: 10px; color: white; font-family: 'Times New Roman', Times, serif; }"
    ".choice{ height: 100px; width: 400px; opacity: 0.7; margin-top: 30px; margin-left: 65%; }"
    ".choice:hover{ opacity: 1; }"
    " form{ position: absolute; height: 50px; width:400px; background: red; outline: 5px solid black; }"
    " .textsec{ margin-top: 10px; margin-left: 30px; border: 3px dotted black; border-radius: 10px; }"
    " b{ margin-left:10px; }"
    " .submitIt{ background: #000fff; color: black; transition: 0.5s ease; margin-top: 10px; margin-left: 20px; border-radius: 10px; border: 3px dotted black; font-family: 'Times New Roman', Times, serif; font-size: 20px; }"
    " .submitIt:hover{ background: black; color:white; letter-spacing: 3px; }"
    " p{ opacity: 1; text-align: center; font-family: Blackoak std; }"
    ".about{height: 80px;width:300px;float: right;margin-left: 30px;margin-right: 30px; margin-top:20px;margin-bottom:10px;color: black;background: white;text-align: center;border: 4px dotted black;opacity: 0.7;}"
    ".instruction{height: 300px;width: 900px; margin-top: 20px;margin-left: 200px;background: linear-gradient(30deg, #12ffdd, #ab3455, #abcdef, #ccffff);border: 3px solid black;border-radius: 15px;}"
    "article{margin-left: 10px; margin-top: 5px;}"
    "marquee{background: black; color: white;transition: 0.5s ease;font-size: 20px;margin-top: 5px; margin-right: 10px;border: 2px dotted white;border-radius: 10px;}"
    "marquee:hover{font-size: 40px; background:blue;}"
    " .foot{text-decoration: none;text-align: center;background: green;margin-top: 20px;margin-right: 200px;margin-left: 200px;text-transform: uppercase;letter-spacing: 10px;color: white;font-size: 20px;padding: 10px;border-radius: 15px;font-family: Copperplate Gothic Bold;}"

    "</style>"
    "</head>"


    "<body>"
    "<h1> BSMRSTU Sports</h1>"
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
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 20; j++)
    {
      lcd.setCursor(j, i);
      lcd.print(" ");
    }
  }
}

void gameConfigure()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game configure Mode");
  lcd.setCursor(0, 1);
  lcd.print("A-Football");
  lcd.setCursor(0, 2);
  lcd.print("B-Badminton");
  lcd.setCursor(0, 3);
  lcd.print("C-Cricket");
  boolean GAME = true;
  char cmd;
  while (GAME) {
    if (Serial.available() > 0)
    {
      cmd = Serial.read();

      if (cmd == 'C')
      {
        lcd.clear();
        lcd.print("You Select Cricket ");
        lcd.setCursor(0, 1);
        lcd.print("Press * confirm");
        lcd.setCursor(0, 2);
        lcd.print("Press C cancel");
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
        lcd.clear();
        lcd.print("You Select Fooball ");
        lcd.setCursor(0, 1);
        lcd.print("Press * confirm");
        lcd.setCursor(0, 2);
        lcd.print("Press C cancel");
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
        lcd.clear();
        lcd.print("You Select Badminton");
        lcd.setCursor(0, 1);
        lcd.print("Press * confirm");
        lcd.setCursor(0, 2);
        lcd.print("Press C cancel");
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
  lcd.setCursor(0, 0);
  lcd.print("                    ");
  lcd.setCursor(0, 0);
  lcd.print("Teams");
  int num = 1;
  pageNum(num);
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
          lcd.clear();
          showTheC();
          wait = false;
        }
        if (football || badminton)
        {
          lcd.clear();
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
        lcd.setCursor(5, 0);
        lcd.print("     ");
        lcd.setCursor(10, 0);
        lcd.print(Secondteam);
      }
      else if (command == '#')
      {
        num++;
        num > 8 ? num = 1 : num = num;
        pageNum(num);
      }
    }
  }
}


void ShowTheIP()
{
  lcd.clear();
  for (int i = 0; i < 20; i++)
  {
    lcd.setCursor(i, 2);
    lcd.print("_");
    delay(200);
  }
  lcd.setCursor(1, 3);
  lcd.print("IP: 192.168.137.20");

}

void showTheC()
{
  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print("1-6 for runs AB-wick");
  lcd.setCursor(0, 3);
  lcd.print("7-dec 0-sw ");
}

void showTheFB()
{
  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print("1,7 ");
  lcd.print(firstteam);
  lcd.print(" 3,9 ");
  lcd.print(Secondteam);
  lcd.setCursor(0, 3);
  lcd.print("D-Reset");
}

void ScoreBoard()
{
  if (cricket)
  {
    lcd.setCursor(0, 0);
    lcd.print(firstteam);
    lcd.print(" ");
    lcd.setCursor(4, 0);
    lcd.print("   ");
    lcd.setCursor(4, 0);
    lcd.print(runsTeamA);
    lcd.print(" / ");
    lcd.setCursor(10, 0);
    lcd.print("   ");
    lcd.setCursor(10, 0);
    lcd.print(wicketTeamA);
    lcd.setCursor(15, 0);
    currentTeam == first_team ? lcd.print("Bat") : lcd.print("   ");

    lcd.setCursor(0, 1);
    lcd.print(Secondteam);
    lcd.print(" ");
    lcd.setCursor(4, 1);
    lcd.print("   ");
    lcd.setCursor(4, 1);
    lcd.print(runsTeamB);
    lcd.print(" / ");
    lcd.setCursor(10, 1);
    lcd.print("   ");
    lcd.setCursor(10, 1);
    lcd.print(wicketTeamB);
    lcd.setCursor(15, 1);
    currentTeam == Second_team ? lcd.print("Bat") : lcd.print("   ");
  }

  else if (badminton || football)
  {
    lcd.setCursor(0, 0);
    lcd.print(firstteam);
    lcd.print(" : ");
    lcd.setCursor(6, 0);
    lcd.print("   ");
    lcd.setCursor(6, 0);
    lcd.print(teamA);

    lcd.setCursor(0, 1);
    lcd.print(Secondteam);
    lcd.print(" : ");
    lcd.setCursor(6, 1);
    lcd.print("   ");
    lcd.setCursor(6, 1);
    lcd.print(teamB);
  }
}

void pageNum(int num)
{
  switch (num) {
    case 1:
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      lcd.setCursor(0, 1);
      lcd.print("ENGINEERING");
      lcd.setCursor(0, 2);
      lcd.print("1.ETE 2.CSE 3.EEE");
      lcd.setCursor(0, 3);
      lcd.print("4.ACCE 5.CE 6.FAE");
      break;

    case 2:
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      lcd.setCursor(0, 1);
      lcd.print("ARCITECTURE");
      lcd.setCursor(0, 2);
      lcd.print("1.ARC               ");
      lcd.setCursor(0, 3);
      lcd.print("                    ");
      break;

    case 3:
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      lcd.setCursor(0, 1);
      lcd.print("SCIENCE");
      lcd.setCursor(0, 2);
      lcd.print("1.MAT 2.STA 3.CHE");
      lcd.setCursor(0, 3);
      lcd.print("4.PHY 5.ESD      ");
      break;

    case 4:
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      lcd.setCursor(0, 1);
      lcd.print("LIFE SCIENCE");
      lcd.setCursor(0, 2);
      lcd.print("1.PHR 2.BGE 3.BMB");
      lcd.setCursor(0, 3);
      lcd.print("4.PSY 5.BOT      ");
      break;

    case 5:
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      lcd.setCursor(0, 1);
      lcd.print("HUMANITIES");
      lcd.setCursor(0, 2);
      lcd.print("1.BAN 2.ENG      ");
      lcd.setCursor(0, 3);
      lcd.print("               ");
      break;

    case 6:
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      lcd.setCursor(0, 1);
      lcd.print("SOCAL SCIENCE");
      lcd.setCursor(0, 2);
      lcd.print("1.SOC 2.IR  3.ECO");
      lcd.setCursor(0, 3);
      lcd.print("4.POS          ");
      break;

    case 7:
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      lcd.setCursor(0, 1);
      lcd.print("BISNUSS");
      lcd.setCursor(0, 2);
      lcd.print("1.MAG 2.AIS 3.MKT");
      lcd.setCursor(0, 3);
      lcd.print("4.FAB 5.THM      ");
      break;

    case 8:
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      lcd.setCursor(0, 1);
      lcd.print("LAW & AGRI");
      lcd.setCursor(0, 2);
      lcd.print("1.LAW             ");
      lcd.setCursor(0, 3);
      lcd.print("2.AGR             ");
      break;
  }
}

void printTheTeam(String teamNumberone, String teamNumbertwo)
{
  lcd.setCursor(5, 0);
  lcd.print(teamNumberone);
  lcd.setCursor(10, 0);
  lcd.print("vs");
  lcd.setCursor(15, 0);
  lcd.print(teamNumbertwo);
}



String ReadTeam(int teamnum)
{
  switch(teamnum){
    case 0:
    return "";
    break;

    case 1:
    return "ETE";
    break;

    case 2:
    return "CSE";
    break;

    case 3:
    return "EEE";
    break;

    case 4:
    return "ACCE";
    break;

    case 5:
    return "CE ";
    break;

    case 6:
    return "FAE";
    break;

    case 7:
    return "ART";
    break;

    case 8:
    return "MAT";
    break;

    case 9:
    return "STA";
    break;

    case 10:
    return "CHE";
    break;

    case 11:
    return "PHY";
    break;

    case 12:
    return "ESD";
    break;

    case 13:
    return "PHR";
    break;

    case 14:
    return "BGE";
    break;

    case 15:
    return "BMB";
    break;

    case 16:
    return "PSY";
    break;

    case 17:
    return "BOT";
    break;

    case 18:
    return "BAN";
    break;

    case 19:
    return "ENG";
    break;

    case 20:
    return "SOC";
    break;

    case 21:
    return "IR ";
    break;

    case 22:
    return "ECO";
    break;

    case 23:
    return "POS";
    break;

    case 24:
    return "MAG";
    break;

    case 25:
    return "AIS";
    break;

    case 26:
    return "MKT";
    break;

    case 27:
    return "FAB";
    break;

    case 28:
    return "THM";
    break;

    case 29:
    return "LAW";
    break;

    
  }
}

int DealWith(String TeamName)
{
  if(TeamName=="ETE"){return 1;}
  else if(TeamName=="CSE"){return 2;}
  else if(TeamName=="EEE"){return 3;}
  else if(TeamName=="ACCE"){return 4;}
  else if(TeamName=="CE "){return 5;}
  else if(TeamName=="FAE"){return 6;}
  else if(TeamName=="ART"){return 7;}
  else if(TeamName=="MAT"){return 8;}
  else if(TeamName=="STA"){return 9;}
  else if(TeamName=="CHE"){return 10;}
  else if(TeamName=="PHY"){return 11;}
  else if(TeamName=="ESD"){return 12;}
  else if(TeamName=="PHR"){return 13;}
  else if(TeamName=="BGE"){return 14;}
  else if(TeamName=="BMB"){return 15;}
  else if(TeamName=="PSY"){return 16;}
  else if(TeamName=="BOT"){return 17;}
  else if(TeamName=="BAN"){return 18;}
  else if(TeamName=="ENG"){return 19;}
  else if(TeamName=="SOC"){return 20;}
  else if(TeamName=="IR "){return 21;}
  else if(TeamName=="ECO"){return 22;}
  else if(TeamName=="POS"){return 23;}
  else if(TeamName=="MAG"){return 24;}
  else if(TeamName=="AIS"){return 25;}
  else if(TeamName=="MKT"){return 26;}
  else if(TeamName=="FAB"){return 27;}
  else if(TeamName=="THM"){return 28;}
  else if(TeamName=="LAW"){return 29;}
  else{return 0;}
}



