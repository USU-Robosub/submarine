<directioncontrol class={ opts.class }>
    <h3 class="title is-3">{ opts.title }</h3>
    <div class={ columns: !opts.horizontal }>
        <div class={ column: !opts.horizontal, 'is-3 is-vertical': !opts.horizontal, 'buttons has-addons': true}>
            <button class="button is-primary is-large" onclick={ increase }>
          <span class="icon is-small">
          <i class={ fas: true,  'fa-angle-double-up': !opts.horizontal, 'fa-angle-double-left': opts.horizontal }></i>
        </span>
        </button>
            <button class="button is-danger is-large" onclick={ stop }>
          <span class="icon is-small">
          <i class="fas fa-stop"></i>
        </span>
        </button>
            <button class="button is-primary is-large" onclick={ decrease }>
          <span class="icon is-small">
          <i class={ fas: true,  'fa-angle-double-down': !opts.horizontal, 'fa-angle-double-right': opts.horizontal }></i>
        </span>
        </button>
        </div>
        <div class={ column: !opts.horizontal }>
            <h4 class="title is-4">Status</h4>
            <p>{ curPower }</p>
        </div>
    </div>
    <script>
        /*global opts */
        let range = opts.range || {}
        range.initial = opts.range.initial || 0
        range.delta = (opts.horizontal?-1:1)*(opts.range.delta || 0.05) || 0.05;
        range.max = opts.range.max || 1
        range.min = opts.range.min || -1
        range.wrapAround = opts.range.wrapAround || false
        let precision = Math.pow(10, opts.range.precision || 2)
        let update = opts.update || ((val) => console.log(opts.title, val))
        this.curPower = range.initial

        increase(){
          this.change(1)
        }

        decrease(){
          this.change(-1)
        }

        change(multiplier) {
          let delta = range.delta * multiplier
          let curPower = this.curPower + delta
          if(isNaN(curPower))
            curPower = delta;
          if (curPower > range.max) {
            if( range.wrapAround ) {
              curPower = range.min
            } else {
              curPower = range.max
            }
          }
          if (curPower < range.min) {
            if( range.wrapAround ) {
              curPower = range.max
            } else {
              curPower = range.min
            }
          }
          if(isNaN(curPower)) {
              curPower = delta
          }
          this.curPower = Math.round(curPower*precision)/precision
          update(this.curPower)
        }

        stop() {
            this.curPower = 'Stopped'
            update(false)
        }
    </script>
</directioncontrol>
