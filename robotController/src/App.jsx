import { useState, useEffect } from 'react'
import './App.css'
import { moveForward, moveBackward, turnLeft, turnRight, stop } from './serverCommands'
const KEY_FORWARD = ['ArrowUp', 'w', 'W']
const KEY_BACKWARD = ['ArrowDown', 's', 'S']
const KEY_LEFT = ['ArrowLeft', 'a', 'A']
const KEY_RIGHT = ['ArrowRight', 'd', 'D']
const KEY_STOP = ' ' // Space key

function App() {
  const [currentAction, setCurrentAction] = useState('Stopped')

  useEffect(() => {
    const handleKeyDown = (event) => {
      if (KEY_FORWARD.includes(event.key)) {
        moveForward()
        setCurrentAction('Moving Forward')
      } else if (KEY_BACKWARD.includes(event.key)) {
        moveBackward()
        setCurrentAction('Moving Backward')
      } else if (KEY_LEFT.includes(event.key)) {
        turnLeft()
        setCurrentAction('Turning Left')
      } else if (KEY_RIGHT.includes(event.key)) {
        turnRight()
        setCurrentAction('Turning Right')
      } else if (event.key === KEY_STOP) {
        stop()
        setCurrentAction('Stopped')
      }
    }

    window.addEventListener('keydown', handleKeyDown)

    return () => {
      window.removeEventListener('keydown', handleKeyDown)
    }
  }, [])

  const handleMoveForward = () => {
    moveForward()
    setCurrentAction('Moving Forward')
  }

  const handleMoveBackward = () => {
    moveBackward()
    setCurrentAction('Moving Backward')
  }

  const handleTurnLeft = () => {
    turnLeft()
    setCurrentAction('Turning Left')
  }

  const handleTurnRight = () => {
    turnRight()
    setCurrentAction('Turning Right')
  }

  const handleStop = () => {
    stop()
    setCurrentAction('Stopped')
  }

  return (
    <>
      <div className="h-full p-8 bg-red-500 overflow-hidden">
        <div className="flex justify-between items-center h-full px-4">
          <div className="flex flex-row space-x-4">
            <button 
              className="bg-white hover:bg-gray-100 text-gray-800 font-bold py-8 px-8 rounded-full shadow text-4xl"
              onClick={handleTurnLeft}
            >
              &#8592;
            </button>
            <button 
              className="bg-white hover:bg-gray-100 text-gray-800 font-bold py-8 px-8 rounded-full shadow text-4xl"
              onClick={handleTurnRight}
            >
              &#8594;
            </button>
          </div>
          <div className="text-white text-2xl font-bold animate-spin">
            {currentAction}
          </div>
          <div className="flex flex-col space-y-4">
            <button 
              className="bg-white hover:bg-gray-100 text-gray-800 font-bold py-8 px-8 rounded-full shadow text-4xl"
              onClick={handleMoveForward}
            >
              &#8593;
            </button>
            <button 
              className="bg-white hover:bg-gray-100 text-gray-800 font-bold py-8 px-8 rounded-full shadow text-4xl"
              onClick={handleStop}
            >
              STOP
            </button>
            <button 
              className="bg-white hover:bg-gray-100 text-gray-800 font-bold py-8 px-8 rounded-full shadow text-4xl"
              onClick={handleMoveBackward}
            >
              &#8595;
            </button>
          </div>
        </div>
      </div>
    </>
  )
}

export default App
