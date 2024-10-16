import { useState, useEffect, useRef } from 'react'
import './App.css'
import { moveForward, moveBackward, turnLeft, turnRight, stopMoving, stopTurning, stop } from './serverCommands'
const KEY_FORWARD = ['ArrowUp', 'w', 'W']
const KEY_BACKWARD = ['ArrowDown', 's', 'S']
const KEY_LEFT = ['ArrowLeft', 'a', 'A']
const KEY_RIGHT = ['ArrowRight', 'd', 'D']
const KEY_STOP = ' ' // Space key

function App() {
  const [currentAction, setCurrentAction] = useState('Stopped')
  const pressedKeys = useRef(new Set())
  const lastActionTime = useRef(0)
  const actionTimeout = useRef(null)

  useEffect(() => {
    const handleKeyDown = (event) => {
      if (!pressedKeys.current.has(event.key)) {
        pressedKeys.current.add(event.key)
        updateAction()
      }
    }

    const handleKeyUp = (event) => {
      pressedKeys.current.delete(event.key)
      updateAction()
    }

    const updateAction = () => {
      const now = Date.now()
      if (now - lastActionTime.current < 200) {
        // If less than 200ms has passed since the last action, schedule the next update
        if (actionTimeout.current) clearTimeout(actionTimeout.current)
        actionTimeout.current = setTimeout(updateAction, 200 - (now - lastActionTime.current))
        return
      }

      const keys = pressedKeys.current
      if (keys.size === 0) {
        stop()
        setCurrentAction('Stopped')
      } else {
        let actions = []
        let isMoving = false
        let isTurning = false

        if (keys.has('ArrowUp') || keys.has('w') || keys.has('W')) {
          moveForward()
          actions.push('Moving Forward')
          isMoving = true
        } else if (keys.has('ArrowDown') || keys.has('s') || keys.has('S')) {
          moveBackward()
          actions.push('Moving Backward')
          isMoving = true
        } else {
          stopMoving()
        }

        if (keys.has('ArrowLeft') || keys.has('a') || keys.has('A')) {
          turnLeft()
          actions.push('Turning Left')
          isTurning = true
        } else if (keys.has('ArrowRight') || keys.has('d') || keys.has('D')) {
          turnRight()
          actions.push('Turning Right')
          isTurning = true
        } else {
          stopTurning()
        }

        if (!isMoving && !isTurning) {
          stop()
          actions.push('Stopped')
        }

        setCurrentAction(actions.join(', '))
      }

      lastActionTime.current = now
    }

    window.addEventListener('keydown', handleKeyDown)
    window.addEventListener('keyup', handleKeyUp)

    return () => {
      window.removeEventListener('keydown', handleKeyDown)
      window.removeEventListener('keyup', handleKeyUp)
      if (actionTimeout.current) clearTimeout(actionTimeout.current)
    }
  }, [])

  const handleButtonDown = (action, actionName) => {
    const now = Date.now()
    if (now - lastActionTime.current >= 200) {
      action()
      setCurrentAction(actionName)
      lastActionTime.current = now
    }
  }

  const handleButtonUp = (stopAction) => {
    stopAction()
    setCurrentAction('Stopped')
  }

  return (
    <>
      <div className="h-full p-8 bg-red-500 overflow-hidden">
        <div className="flex justify-between items-center h-full px-4">
          <div className="flex flex-row space-x-4">
            <button 
              className="bg-white hover:bg-gray-100 text-gray-800 font-bold py-8 px-8 rounded-full shadow text-4xl"
              onMouseDown={() => handleButtonDown(turnLeft, 'Turning Left')}
              onMouseUp={() => handleButtonUp(stopTurning)}
              onMouseLeave={() => handleButtonUp(stopTurning)}
            >
              &#8592;
            </button>
            <button 
              className="bg-white hover:bg-gray-100 text-gray-800 font-bold py-8 px-8 rounded-full shadow text-4xl"
              onMouseDown={() => handleButtonDown(turnRight, 'Turning Right')}
              onMouseUp={() => handleButtonUp(stopTurning)}
              onMouseLeave={() => handleButtonUp(stopTurning)}
            >
              &#8594;
            </button>
          </div>
          <div className="text-white text-2xl font-bold animate-bounce">
            {currentAction}
          </div>
          <div className="flex flex-col space-y-4">
            <button 
              className="bg-white hover:bg-gray-100 text-gray-800 font-bold py-8 px-8 rounded-full shadow text-4xl"
              onMouseDown={() => handleButtonDown(moveForward, 'Moving Forward')}
              onMouseUp={() => handleButtonUp(stopMoving)}
              onMouseLeave={() => handleButtonUp(stopMoving)}
            >
              &#8593;
            </button>
            <button 
              className="bg-white hover:bg-gray-100 text-gray-800 font-bold py-8 px-8 rounded-full shadow text-4xl"
              onClick={() => handleButtonDown(stop, 'Stopped')}
            >
              STOP
            </button>
            <button 
              className="bg-white hover:bg-gray-100 text-gray-800 font-bold py-8 px-8 rounded-full shadow text-4xl"
              onMouseDown={() => handleButtonDown(moveBackward, 'Moving Backward')}
              onMouseUp={() => handleButtonUp(stopMoving)}
              onMouseLeave={() => handleButtonUp(stopMoving)}
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
