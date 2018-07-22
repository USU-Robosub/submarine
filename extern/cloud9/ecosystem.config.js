module.exports = {
  apps: [
    {
      name: "cloud9",
      cwd: "../../../cloud9",
      args: "--port 4967 --listen 127.0.0.1 --collab -w /home/pi",
      script: "../../../cloud9/server.js"
    }
  ]
}
