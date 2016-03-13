#include <Servo.h>
#include <Mailbox.h>

//define global static variables
#define STATE_WAITING 0
#define STATE_SWEEP 1
#define NUMBER_OF_SHOOTS 5 //how many time we must shoot the player
#define FIRST_PLAYER_POS_START 40 //servo start position for the first player
#define FIRST_PLAYER_POS_END 60 //servo end position for the first player
#define SECOND_PLAYER_POS_START 80
#define SECOND_PLAYER_POS_END 100
#define THIRD_PLAYER_POS_START 120
#define THIRD_PLAYER_POS_END 140



Servo myservo;
int pos = 0;
int currentState = 0;
 
void setup() {
  Bridge.begin(); // Required to recibe messages from linino
  Console.begin();  // Required to send messages to the console
  
  myservo.attach(9);
}

void loop() {

checkNewMessage();

//execute the current state tasks
  switch(currentState){
    case STATE_WAITING:
      break; //do nothing
    case STATE_SWEEP:
      sweep();
      break;
  }
}

void sweep(){
    for(pos = 0; pos <= 180; pos +=1){
      myservo.write(pos);
      delay(8);
    }

    checkNewMessage();
    
    for(pos = 180; pos >= 0; pos -= 1){
      myservo.write(pos);
      delay(8);
    } 
}

void shootPlayer(int playerNr){
  int start = 0;
  int finish = 0;
  
  switch(playerNr){
    case 1:
      start = FIRST_PLAYER_POS_START;
      finish = FIRST_PLAYER_POS_END;
      break;
    case 2:
      start = SECOND_PLAYER_POS_START;
      finish = SECOND_PLAYER_POS_END;
      break;
    case 3:
      start = THIRD_PLAYER_POS_START;
      finish = THIRD_PLAYER_POS_END;
      break;
  }

  pos = start + (finish - start); //move the servo to the center position of the target range
  myservo.write(pos); 
  //HERE WE MUST TURN THE PUMP ON
  for(int i = 0; i <= NUMBER_OF_SHOOTS; i++){
    for (pos = start; pos <= finish; pos++){
      myservo.write(pos);
      delay(5);
    }
    for(pos = finish; pos >= start; pos--){
      myservo.write(pos);
      delay(5);
    }  
  }
  //HERE WE MUST TURN THE PUMP OFF
  currentState = STATE_SWEEP;

}

void shootAll(){
  int start = FIRST_PLAYER_POS_START;
  int finish = THIRD_PLAYER_POS_END;
  
  //HERE WE MUST TURN THE PUMP ON
   for(int i = 0; i <= 2; i++){ //we limit the ammount of shoot to 2 
    for (pos = start; pos <= finish; pos++){
      myservo.write(pos);
      delay(5);
    }
    for(pos = finish; pos >= start; pos--){
      myservo.write(pos);
      delay(5);
    }  
  }
    
  //HERE WE MUST TURN THE PUMP OFF
  currentState = STATE_SWEEP;
}

void checkNewMessage(){
  String message;
// Mailbox control
if (Mailbox.messageAvailable()){
  Mailbox.readMessage(message);
  Console.println("Message = " +message);

  String action = message.substring(0,message.indexOf(" "));
  Console.println("Action: " +  action);  
  String command;
  String command_parameters;

  if(action == "sweep"){
    currentState = STATE_SWEEP;
  }else if(action == "wait"){
    currentState = STATE_WAITING;
  }else if(action == "shoot"){
    command = message.substring(message.indexOf(" ")+1, message.length());
    Console.println("Command do: " + command);
    shootPlayer(command.toInt());
  }else if(action == "shootAll"){
    shootAll();
  }
  
}

}

