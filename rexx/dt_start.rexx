/* start DeliTracker */

parse arg filename

if show('P','DELITRACKER') == '0' then do

  address 'COMMAND'

  "run <>NIL: AmiKit:Utilities/DeliTracker_II/DeliTracker2"

  "waitforport DELITRACKER"

end

address 'DELITRACKER'

playmod filename

