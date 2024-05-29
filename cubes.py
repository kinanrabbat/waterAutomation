import sys 
from time import sleep
from gpiozero import OutputDevice as stepper
from gpiozero import LED

pin = LED(27)

if(len(sys.argv) == 3):
	time = int(sys.argv[1]) 
	cubeNum = int(sys.argv[2])
	
if cubeNum == 1: 
    IN1 = stepper(12)
    IN2 = stepper(16)
    IN3 = stepper(20)
    IN4 = stepper(21)

elif cubeNum == 2:
    IN1 = stepper(2)
    IN2 = stepper(3)
    IN3 = stepper(4)
    IN4 = stepper(17)
        
elif cubeNum == 3:
    IN1 = stepper(6)
    IN2 = stepper(13)
    IN3 = stepper(19)
    IN4 = stepper(26)

stepPins = [IN1,IN2,IN3,IN4] # Motor GPIO pins</p><p>
stepDir = -1        # Set to 1 for clockwise
seq = [[1,0,0,0], # Define step sequence as shown in manufacturers datasheet
             [0,1,0,0],
             [0,0,1,0],
             [0,0,0,1]]
stepCount = len(seq)
waitTime = 0.004

stepCounter = 0  

def right90():
  stepCounter = 0  
  rotCount = 512
  while(rotCount != 0):
      for pin in range(0,4):
        xPin=stepPins[pin]          # Get GPIO
        if seq[stepCounter][pin]!=0:
          xPin.on()
        else:
          xPin.off()
      stepCounter += stepDir
      rotCount -= 1
      if (stepCounter >= stepCount):
        stepCounter = 0
      if (stepCounter < 0):
        stepCounter = stepCount+stepDir
      sleep(waitTime)     # Wait before moving on
      
def left90():
  stepCounter = 0  
  rotCount = 512
  while(rotCount != 0):
      for pin in range(0,4):
        xPin=stepPins[pin]          # Get GPIO
        if seq[stepCounter][pin]!=0:
          xPin.on()
        else:
          xPin.off()
      stepCounter -= stepDir
      rotCount -= 1
      if (stepCounter >= stepCount):
        stepCounter = 0
      if (stepCounter < 0):
        stepCounter = stepCount+stepDir
      sleep(waitTime)     # Wait before moving on

right90()
pin.on()
sleep(time - 0.2)
pin.off()
sleep(0.2)
left90()
