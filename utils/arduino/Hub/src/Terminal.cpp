#include <Terminal.hpp>

#define CLEAR_HEIGHT 50

Terminal::Terminal(DebugWrapper* wrappers, int32_t controlCode)
  : code(controlCode),
    finished(true),
    commandIndex(){

}

bool Terminal::handleControlCode(){
  if(this->finished){
    this->finished = false;
    Serial.println(TERMINAL_HEADER);
    Serial.print(TERMINAL_PROMPT);
  }
  if(Serial.available()){
    char input = Serial.read();
    if(input == 10){
      this->currentCommand[this->commandIndex] = 0;
      String command(this->currentCommand);
      if(command.equals("exit")){
        this->finished = true;
      }else if(command.equals("clear")){
        for(unsigned int i = 0; i < CLEAR_HEIGHT; ++i){
          Serial.println();
        }
      }else if(command.equals("help")){
        Serial.println("Keys:");
        Serial.println("  press [`] for a new prompt");
        Serial.println("  press [ENTER] to run the entered command");
        Serial.println("Commands:");
        Serial.println("  clear - create some empty space above the next prompt");
        Serial.println("  help - show available commands");
        Serial.println("  exit - close the terminal, and return to binary input");
      }
      this->commandIndex = 0;

      if(!this->finished){
        Serial.print("debug@Arduino$ ");
      }
    }else{
      this->currentCommand[this->commandIndex++] = input;
      Serial.print(input);
    }
  }

  return !this->finished;
}

int32_t Terminal::controlCode(){
  return this->code;
}
