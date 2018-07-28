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
        opts.initial = opts.initial || 0
        opts.delta = (opts.horizontal?-1:1)*(opts.delta || 0.05);
        opts.maxRange = opts.maxRange || 1
        opts.minRange = opts.minRange || -1
        let precision = Math.pow(10, opts.precision || 2)
        opts.update = opts.update || ((val) => console.log(opts.title, val))
        this.curPower = opts.initial

        increase() {
            this.curPower = Math.round((this.curPower + opts.delta) * precision) / precision
            if (this.curPower > opts.maxRange)
                this.curPower = opts.minRange
            if(isNaN(this.curPower)) {
                this.curPower = opts.delta
            }
            opts.update(this.curPower)
        }

        decrease() {
            this.curPower = Math.round((this.curPower - opts.delta) * precision) / precision
            if (this.curPower < opts.minRange)
                this.curPower = opts.maxRange
            if(isNaN(this.curPower)) {
                this.curPower = -opts.delta
            }
            opts.update(this.curPower)
        }

        stop() {
            this.curPower = 'Stopped'
            opts.update(false)
        }
    </script>
</directioncontrol>