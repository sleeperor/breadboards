from time import sleep
from serial import Serial

prev = [0, 0, 0, 0, 0, 0, 0]


with Serial('/dev/ttyACM0', 19200, timeout=1) as ttyACM0:
  while True:
    with open('/proc/stat') as stat:
      current = map(lambda x: int(x), stat.readline().split()[1:-3])
      diff = map(lambda x, y: x - y, current, prev)
      prev = current
      time_all = sum(diff, 0)
      time_idle = diff[3]
      time_busy = time_all - time_idle

      val_busy = ((time_all - time_idle) * 256) / time_all
      
      ttyACM0.write(str(val_busy))
      
    sleep(0.1)

