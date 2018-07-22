#include <Threading/ProcessChild.hpp>

Threading::ProcessChild::ProcessChild(std::string command, std::vector<std::string> arguments)
  : parentToChild(-1),
    childFromParent(-1),
    parentFromChild(-1),
    childToParent(-1),
    childPID(-1){
  int fdIn[2], fdOut[2];
  if(pipe(fdIn) < 0 || pipe(fdOut) < 0){
  	/* FATAL: cannot create pipe */
  	/* close readpipe[0] & [1] if necessary */
    std::cout << "Error pipe" << std::endl;
    throw "Error pipe";
  }
  this->parentToChild = fdIn[1];
  this->parentFromChild = fdOut[0];
  this->childFromParent = fdIn[0];
  this->childToParent = fdOut[1];
  auto pid = fork();
  if(pid < 0){
    std::perror("Error");
  }else if(pid == 0){
    this->childProcess(command, arguments);
  }else{
    this->parentProcess(pid);
  }
}

Threading::ProcessChild::~ProcessChild(){
  close(this->parentToChild);
  close(this->parentFromChild);
  int status;
  waitpid(this->childPID,&status,0);
}

void Threading::ProcessChild::parentProcess(pid_t pid){
  this->childPID = pid;
  close(this->childFromParent);
  close(this->childToParent);
}

void Threading::ProcessChild::childProcess(std::string command, std::vector<std::string> arguments){
  close(this->parentToChild);
  close(this->parentFromChild);
  if(dup2(this->childToParent, STDOUT_FILENO) == -1) {
    throw "Failed to dup2 stdout";
  }
  if(dup2(STDOUT_FILENO, STDERR_FILENO) == -1) {
    throw "Failed to dup2 stderr";
  }
  if(dup2(this->childFromParent, STDIN_FILENO) == -1) {
    throw "Failed to dup2 stdin";
  }
  close(this->childFromParent);
  close(this->childToParent);
  const char** cArguments = new const char*[arguments.size() + 2];
  cArguments[0] = command.c_str();
  cArguments[arguments.size() + 1] = NULL;
  std::transform(arguments.begin(), arguments.end(), cArguments + 1, Threading::ProcessChild::convertArguments);
  (void) execvp(command.c_str(), const_cast<char* const*>(cArguments));
  std::perror(command.c_str());
  throw "Failed to execvp [command]";
}

int Threading::ProcessChild::read(unsigned char* buffer, std::size_t length){
  return ::read(this->parentFromChild, buffer, length);
}

int Threading::ProcessChild::write(const unsigned char* buffer, std::size_t length){
  return ::write(this->parentToChild, buffer, length);
}

char* Threading::ProcessChild::convertArguments(const std::string& s)
{
   char *pc = new char[s.size()+1];
   std::strcpy(pc, s.c_str());
   return pc;
}
