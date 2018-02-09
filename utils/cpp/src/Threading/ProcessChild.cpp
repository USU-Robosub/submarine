// #include <Threading/ProcessChild.h>
//
// StreamEye::StreamEye() {
//   pipe(m_fdIn);
//   pipe(m_fdOut);
//   auto pid = fork();
// 	if(pid < 0){
// 		std::perror("Error");
// 	}else if(pid==0){
// 		this->childProcess(pid);
// 	}else{
//     this->parentProcess(pid);
// 	}
// }
//
// void StreamEye::parentProcess(pid_t pid){
//   m_PID = pid;
// }
//
// void StreamEye::childProcess(){
//   dup2(m_fdIn[0],STDIN_FILENO);
//   dup2(m_fdIn[1],STDOUT_FILENO);
//   close(m_fdIn[0]);
//   close(m_fdIn[1]);
//   close(m_fdOut[0]);
//   close(m_fdOut[1]);
//   execvp("streameye","streameye","-q","-p","8080","-s",SEPERATOR);
//   std::cerr << "-tosh: ";
//   std::perror(command.at(0).c_str());
//   exit(1);
// }
//
// void StreamEye::writeJpeg(std::vector<uchar> image) {
//   write(m_fdIn[1], SEPERATOR, sizeof(SEPERATOR)-1);
//   write(m_fdIn[1], (char *) &image[0], sizeof(uchar)*image.size());
// }
//
// ~StreamEye::StreamEye() {
//   int status;
//   close(m_fdIn[0]);
//   close(m_fdIn[1]);
//   waitpid(m_PID,&status,0);
//   close(m_fdOut[0]);
//   close(m_fdOut[1]);
// }
