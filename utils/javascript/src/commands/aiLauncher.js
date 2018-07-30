const { Command } = require('../scheduler')
const { map } = require('rxjs/operators')
const { Subject, pipe } = require('rxjs')
const { parallel } = require('../scheduler/tools')

const aiLauncher = aiCommand => Command()
  .named('AI launcher')
  .require('killSwitch')
  .action((system, scheduler, log) => {
    const activeInstance = null
    return system.killSwitch.status().pipe(
      parallel(
        pipe(
          filter(status => status == true),
          map(() => {
            if(activeInstance != null){
              log.warn('Detected kill switch activation. AI script already running. Taking no action')
              return // return without doing anything
            }
            log.good('Detected kill switch activation. Starting AI script', instance.errors)
            activeInstance = scheduler.build(aiCommand)
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
            if(activeInstance != null){
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
