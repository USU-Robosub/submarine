<controls class="tile, is-ancestor, is-vertical">
    <div each={ row in opts.controls } class="tile">
        <div each={ control in row } class="tile is-parent">
          <directioncontrol if={ control.type == "directioncontrol" || !control.type } class="tile is-child box" title={ control.title } range={ control.range || {} } update={ control.update } horizontal={ control.horizontal }></directioncontrol>
          <pidcontrol if={ control.type == "pidcontrol" } class="tile is-child box" title={ control.title } update={ control.update }></pidcontrol>
        </div>
    </div>
    <!-- <div class="tile is-parent">
      <pidcontrol class="tile is-child box" title="Heading PID Gains" update={  }></pidcontrol>
    </div> -->
</controls>
