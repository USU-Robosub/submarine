#include <Streameye.h>
int ofile = -1;

StreamEye::StreamEye() {
  ofile = open("log.txt",O_WRONLY|O_CREAT);
  if ( pipe(m_fdIn) < 0  ||  pipe(m_fdOut) < 0 ){
  	/* FATAL: cannot create pipe */
  	/* close readpipe[0] & [1] if necessary */
    std::cout << "Error pipe" << std::endl;
  }
  std::cout << m_fdIn[0] << ":" << m_fdIn[1] << ":" << m_fdOut[0] << ":" << m_fdOut[1] << ":" << std::endl;
  auto pid = fork();
	if(pid < 0){
		std::perror("Error");
	}else if(pid==0){
		this->childProcess();
	}else{
    m_PID = pid;
    close(m_fdIn[0]);
    close(m_fdOut[1]);
    close(ofile);
    char nChar;
    // while(read(m_fdOut[0], &nChar, 1)==1) {
    //   std::cout << "Char:" << nChar << std::endl;
    //   write(STDOUT_FILENO,&nChar, 1);
    // }
	}
}

void StreamEye::childProcess(){
  // dup2(m_fdIn[0],STDIN_FILENO);
  // dup2(m_fdOut[1],STDOUT_FILENO);
  close(m_fdIn[1]);
  close(m_fdOut[0]);
  // if(dup2(m_fdOut[1],STDOUT_FILENO) == -1) {
  //   exit(errno);
  // }

  // if(dup2(ofile,STDOUT_FILENO) == -1) {
  //    exit(errno);
  // }
  // if(dup2(STDOUT_FILENO,STDERR_FILENO) == -1) {
  //    exit(errno);
  // }
  if(dup2(m_fdIn[0],STDIN_FILENO) == -1) {
    exit(errno);
  }
  close(ofile);
  // close(m_fdIn[0]);
  // close(m_fdOut[1]);
  std::cout << "Dup2 done" << std::endl;
  char* const  cmd[] = { "streameye","-q","-p","8080","-s",SEPERATOR,NULL };
  //char* const  cmd[] = { "echo"/*,"-q"*/,"-p","8080","-s",SEPERATOR,NULL };
  int result = execvp(cmd[0], &cmd[0]);
  std::perror(cmd[0]);
  exit(result);
}

void StreamEye::writeJpeg(std::vector<unsigned char> image) {
  write(m_fdIn[1], SEPERATOR, sizeof(SEPERATOR)-1);
  write(m_fdIn[1], (char *) &image[0], sizeof(unsigned char)*image.size());
}

StreamEye::~StreamEye() {
  int status;
  //close(m_fdIn[0]);
  close(m_fdIn[1]);
  close(m_fdOut[0]);
  close(ofile);
  //close(m_fdOut[1]);
  std::cout << "Closed Pipes" << std::endl;
  waitpid(m_PID,&status,0);
  std::cout << "Process finished" << std::endl;
}
