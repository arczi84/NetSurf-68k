/* start DeliTracker */

options
parse arg player filename

DeliTracker = player

if show('P','DELITRACKER') == '0' then do

  address 'COMMAND'

  "run " DeliTracker

  "sys:rexxc/waitforport DELITRACKER"
  
end

address 'DELITRACKER'

filename=strip(filename)
playmod filename
