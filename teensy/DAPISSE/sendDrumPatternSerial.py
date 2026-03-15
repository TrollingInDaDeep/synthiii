from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
import time
import serial

comport = "COM3" #edit your com port here
baudrate = 115200 #edit your baud rate here
filepath = "D:/NAS/Projekte/Synth/synthiii/teensy/DAPISSE/drumPatterns.txt" #edit your file path here
filedir = "D:/NAS/Projekte/Synth/synthiii/teensy/DAPISSE/" #just the file directory location to watch
ser = serial.Serial(comport, baudrate, timeout=1)
print(f"Serial Port opened")

# called when the watchdog triggers an event
class MyEventHandler(FileSystemEventHandler):
	# called when file is modified
	def on_modified(self, event): 
		print(f"File modified: {event.src_path}")
		ser.write(open(filepath, "rb").read()) #actually write the file to serial
	

if __name__ == "__main__":
	event_handler = MyEventHandler()
	observer = Observer()
	observer.schedule(event_handler, filedir, recursive=True)

    # Start the observer
	observer.start()
	print(f"Monitoring dir: {filedir}")

	try:
		while True:
			time.sleep(1)
	except KeyboardInterrupt:
		observer.stop()
		ser.close()
	observer.join()



