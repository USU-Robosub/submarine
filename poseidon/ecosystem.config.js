module.exports = {
  apps: [
    {
      name: "system",
      cwd: "system",
      script: "./system/run.sh"
    },
    {
      name: "agent",
      cwd: "agent",
      script: "./agent/run.sh"
    }
  ]
}
