#include <Hub.hpp>

Hub::Hub(Controller** controllers, int numControllers, int maxReadsPerPoll)
: _controllers(controllers)
, _numControllers(numControllers)
, state(MessageState::CHECK)
, currentMessage()
, dataLeft(0)
, maxReadsPerPoll(maxReadsPerPoll)
, terminalActive(false)
, commandIndex(0)
{
  Serial.begin(115200);//115200
  while(!Serial){}
}

#define TERMINAL_SIGNAL_2 1836213620 // "term"
#define TERMINAL_SIGNAL 1953789044 // "t" 4 times

void Hub::serveEvent(){
  if(this->currentMessage.name < this->_numControllers){
    _controllers[this->currentMessage.name]->execute(this, this->currentMessage.data, this->currentMessage.length);
  }
}

void Hub::poll()
{
  if(this->terminalActive){
    // while(Serial.available()){
    //   char input = Serial.read();
    //   if(input == 10 || input == 96){
    //     Serial.println();
    //     if(input == 96){ // ingore the command and start a new one
    //       this->commandIndex = 0;
    //     }
    //     this->currentCommand[this->commandIndex] = 0;
    //     String command = this->currentCommand;
    //     if(command.equals("blink")){
    //       pinMode(13, OUTPUT);
    //       digitalWrite(13, HIGH);
    //       delay(500);
    //       digitalWrite(13, LOW);
    //       delay(500);
    //       digitalWrite(13, HIGH);
    //       delay(500);
    //       digitalWrite(13, LOW);
    //     }else if(command.equals("exit")){
    //       Serial.println("bye!");
    //       this->terminalActive = false;
    //     }else if(command.equals("freeze disable")){
    //       Serial.println("[Warning] all safety systems have been disabled");
    //     }else if(command.equals("freeze enable")){
    //       Serial.println("[Info] all safety systems have been enabled");
    //     }else if(command.equals("exit")){
    //       Serial.println("bye!");
    //       this->terminalActive = false;
    //     }else if(command.equals("clear")){
    //       for(unsigned int i = 0; i < 50; ++i){
    //         Serial.println();
    //       }
    //     }else if(command.equals("echo")){
    //       char buffer[100];
    //       int bufferIndex = 0;
    //       Serial.print("Message: ");
    //       while(buffer[bufferIndex - 1] != 10){
    //         if(Serial.available()){
    //           buffer[bufferIndex++] = Serial.read();
    //           Serial.print(buffer[bufferIndex - 1]);
    //         }
    //       }
    //       buffer[bufferIndex - 1] = 0;
    //       Serial.println(buffer);
    //     }else if(command.equals("help")){
    //       Serial.println("Keys:");
    //       Serial.println("  press [`] for a new prompt");
    //       Serial.println("  press [ENTER] to run the command");
    //       Serial.println("Commands:");
    //       Serial.println("  freeze disable - force all controllers to stay unfrozen");
    //       Serial.println("  freeze enable - allow controllers to be frozen");
    //       Serial.println("  blink - blink the onboard LED");
    //       Serial.println("  echo - echo back a message");
    //       Serial.println("  clear - create some empty space above the next prompt");
    //       Serial.println("  help - show available commands");
    //       Serial.println("  exit - close the terminal, and return to binary input");
    //     }else if(this->commandIndex > 0){ // not just an enter
    //       Serial.println("No command '" + command + "' found");
    //     }
    //     this->commandIndex = 0;
    //
    //     if(this->terminalActive){
    //       Serial.print("debug@Arduino$ ");
    //     }
    //   }else if(input >= 32 && input <= 126){
    //     Serial.print(input);
    //     this->currentCommand[this->commandIndex++] = input;
    //   }
    // }
  }
  int count = 0;
  while(Serial.available() >= 4 && count < this->maxReadsPerPoll && !this->terminalActive){
    ++count;
    switch(this->state){
      case MessageState::CHECK:
        this->currentMessage.check = this->readInt();
        // Serial.println(this->currentMessage.check); get now terminal_signal
        if(this->currentMessage.check == 0){
          this->state = MessageState::NAME;
        }else if(this->currentMessage.check == TERMINAL_SIGNAL || this->currentMessage.check == TERMINAL_SIGNAL_2){
          // Serial.println("================================");
          // Serial.println("= USU Robosub Arduino Terminal =");
          // Serial.println("================================");
          // Serial.print("debug@Arduino$ ");
          this->terminalActive = true;
        }
        break;
      case MessageState::NAME:
        this->currentMessage.name = this->readInt();
        this->state = MessageState::LENGTH;
        break;
      case MessageState::LENGTH:
        this->currentMessage.length = this->readInt();
        this->dataLeft = this->currentMessage.length;
        if(this->currentMessage.data != nullptr){
          delete this->currentMessage.data;
          this->currentMessage.data = nullptr;
        }
        if(this->dataLeft > 1000){
          this->state = MessageState::CHECK;
        }else if(this->dataLeft > 0){
          this->currentMessage.data = new int32_t[this->currentMessage.length];
          this->state = MessageState::DATA;
        }else{
          this->serveEvent();
          this->state = MessageState::CHECK;
        }
        break;
      case MessageState::DATA:{
        int32_t dataIndex = this->currentMessage.length - this->dataLeft;
        this->currentMessage.data[dataIndex] = this->readInt();
        --this->dataLeft;
        if(this->dataLeft == 0){
          this->serveEvent();
          this->state = MessageState::CHECK;
        }
        break;
      }
      case MessageState::ALIGNING:
        //Not sure what should be done here but it is complaining that it is not implemented
        break;
    }
  }
}

void Hub::emit(int32_t name, int32_t* data, int32_t length)
{
  int32_t null = 0;
  writeInt(null);
  writeInt(name);
  writeInt(length);
  for(int32_t i = 0; i < length; ++i){
    writeInt(data[i]);
  }
}

int32_t Hub::readInt()
{
  return (this->read()) |
         (this->read() << 8) |
         (this->read() << 16) |
         (this->read() << 24);
}

void Hub::writeInt(int32_t value)
{
  Serial.write((unsigned char*)&value, sizeof(int32_t));
}

int32_t Hub::read(){
  return Serial.read();
}

void Hub::kill(){
  for(int32_t i = 0; i < _numControllers; i++)  {
    _controllers[i]->kill();
  }
}

void Hub::restart()
{
  for(int32_t i = 0; i < _numControllers; i++)  {
    _controllers[i]->restart();
  }
}