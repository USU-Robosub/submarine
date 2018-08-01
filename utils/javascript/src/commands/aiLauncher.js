const { Command } = require('../scheduler')
const { map, filter } = require('rxjs/operators')
const { Subject, pipe } = require('rxjs')
const { parallel, sequential } = require('../scheduler/tools')

const aiLauncher = (aiCommand, preRunCommand) => Command()
  .named('AI launcher')
  .require('killSwitch')
  .action((system, scheduler, log) => {
    let activeInstance = null
    return system.killSwitch.status().pipe(
      parallel(
        pipe(
          filter(status => status == true),
          map(() => {
            if(activeInstance != null){
              log.warn('Detected kill switch activation. AI script already running. Taking no action')
              return // return without doing anything
            }
            log.good('Detected kill switch activation. Starting AI script')
            activeInstance = scheduler.build(sequential(preRunCommand, aiCommand))
              .then(() => log.info('AI script ended')) // pre binds the promise for the error handler
              .run()
            if(activeInstance.errors){
              log.error('Failed to start AI script', activeInstance.errors)
            }else{
              log.info('Started AI script')
              activeInstance.to.promise().catch(e => {
                log.error('Failed when running AI script', e)
              })
            }
          })
        ),
        pipe(
          filter(status => status == false),
          map(() => {
            if(activeInstance == null){
              log.warn('Detected kill switch deactivation. AI script not running. Taking no action')
              return // return without doing anything
            }
            log.error('Detected kill switch deactivation. Forcing AI script to stop')
            activeInstance.abort()
            activeInstance = null
          })
        )
      )
    )
  })

module.exports = {
  aiLauncher
}
