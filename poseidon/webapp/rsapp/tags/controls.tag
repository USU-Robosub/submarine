<controls class="tile, is-ancestor, is-vertical">
    <div each={ row in opts.controls } class="tile">
        <div each={ control in row } class="tile is-parent">
            <directioncontrol class="tile is-child box" title={ control.title } update={ control.update } horizontal={ control.horizontal }></directioncontrol>
        </div>
    </div>
</controls>