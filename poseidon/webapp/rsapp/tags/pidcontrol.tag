<pidcontrol class={ opts.class }>
    <h3 class="title is-3">{ opts.title }</h3>
    <div>
        <div class="level is-mobile">
          <div class="level-item field is-horizontal">
            <div class="field-label is-normal">
              <label class="label">P:</label>
            </div>
            <div class="field-body">
              <div class="field">
                <p class="control">
                  <input ref="pGain" type="text" class="input is-primary is-large" value={ this.p }>
                </p>
              </div>
            </div>
          </div>
          <div class="level-item field is-horizontal">
            <div class="field-label is-normal">
              <label class="label">I:</label>
            </div>
            <div class="field-body">
              <div class="field">
                <p class="control">
                  <input ref="iGain" type="text" class="input is-primary is-large" value={ this.i }>
                </p>
              </div>
            </div>
          </div><div class="level-item field is-horizontal">
            <div class="field-label is-normal">
              <label class="label">D:</label>
            </div>
            <div class="field-body">
              <div class="field">
                <p class="control">
                  <input ref="dGain" type="text" class="input is-primary is-large" value={ this.d }>
                </p>
              </div>
            </div>
          </div>
        </div>
        <button class="button is-primary is-large is-fullwidth" onclick={ setGains }>
          <span class="icon is-small">
          <i class="far fa-save"></i>
        </span>
        <span>
        Save
      </span>
        </button>
    </div>
    <style scoped>
      .level div.level-item{
        flex-shrink: 1;
      }
    </style>
    <script>
        /*global opts */
        opts.update = opts.update || ((val) => console.log(opts.title, val))

        this.p = opts.p || 1
        this.i = opts.i || 0
        this.d = opts.d || 0

        setGains() {
            this.p = parseFloat(this.refs.pGain.value || 0)
            this.i = parseFloat(this.refs.iGain.value || 0)
            this.d = parseFloat(this.refs.dGain.value || 0)
            opts.update(parseFloat(this.p), parseFloat(this.i), parseFloat(this.d))
        }
    </script>
</pidcontrol>
