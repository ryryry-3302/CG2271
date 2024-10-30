import { useState, useEffect, useRef } from 'react'
import './App.css'
import { sendBuzzer, sendCode, sendHardStop } from './serverCommands'
import { Joystick } from 'react-joystick-component';

function App() {

    const [speed, setSpeed] = useState(0)
    const [angle, setAngle] = useState(0)
    const [joyStickCode, setJoyStickCode] = useState(0)
    const [isBuzzerSpinning, setIsBuzzerSpinning] = useState(false)
    const [serverIP, setServerIP] = useState('')
    const [oldCode, setOldCode] = useState(0)

    const handleMove = (e) => {
        // setspeed(Math.round((e.x+1) * 3))
        // setangle(Math.round((e.y+1) * 3.5))
        const codeToSend = calculateCode(e.x, e.y)
        if (true) {
            (sendCode(codeToSend, serverIP))
        } else {
            console.log("No change")
        }
    }

    const sqrt2 = Math.sqrt(2);

    const speedMapping = {
        1: 1,
        2: 2,
        3: 4,
        4: 4
        }

    const calculateCode = (x, y) => {
        //a number from 0 to 63
        const speed = Math.sqrt((x * x) + (y * y))/sqrt2;
        const speed_scaled = Math.min(Math.round(speed * 4), 4);
        const angle = Math.atan2(x, -y);
        const percentage_angle = (angle-Math.PI)/(2*Math.PI) * -1;
        const scaled_angle = Math.min(Math.round(percentage_angle * 16), 16);

        const code =  speed_scaled * 16 + scaled_angle
        setJoyStickCode(code)
        setSpeed(speed_scaled)
        setAngle(scaled_angle)
        return Math.min(63, code)
    }

    const handleStop = (e) => {
        setSpeed(0)
        setAngle(0)
        calculateCode(0, 0)
        sendHardStop(serverIP)
        const codeToSend = calculateCode(e.x, e.y)
        if (true) {
            (sendCode(codeToSend, serverIP))
        } else {
            console.log("No change")
        }
        sendHardStop(serverIP)
        setTimeout(sendHardStop(serverIP), 25)
        setTimeout(sendHardStop(serverIP), 50)

        // let sendCodeThing = sendCode(0, serverIP)
        // while (sendCodeThing === false){
        //     //to make sure stop is sent
        //     sendCodeThing = sendCode(0, serverIP)
        // }
    }

    const handleBuzzer = () => {
        setIsBuzzerSpinning(true)
        console.log("Buzzer")
        sendBuzzer(serverIP)   
        speakText("Hello this is ISA many time call no answer. You have one urgent message. Please press one for enlish, or 2 for chinese")
        setTimeout(() => {
            setIsBuzzerSpinning(false)
        }, 10000)
    }

    const speakText = (text) => {
        const utterance = new SpeechSynthesisUtterance(text);
        window.speechSynthesis.speak(utterance);
    };
      
    const handleIPChange = (e) => {
        setServerIP(e.target.value)
        console.log(serverIP)
    }
    const backgroundStyle = {
        backgroundImage: `url('https://static01.nyt.com/images/2016/05/30/us/30gorilla-web1/30gorilla-web1-superJumbo.jpg')`,
        backgroundSize: 'cover',
        backgroundPosition: 'center',
        backgroundRepeat: 'no-repeat'
    };

    return (
        <>
            <div className='bg-yellow-500 h-screen w-screen p-16 flex flex-col justify-center items-center' style={backgroundStyle}>
                <input 
                    type="text" 
                    value={serverIP} 
                    onChange={handleIPChange}
                    placeholder="Enter server IP address"
                    className='mb-4 p-2 text-lg rounded-md'
                />
                <div className='flex justify-center items-center h-fit'>
                    <Joystick size={288 } sticky={false} baseColor="red" stickColor="blue" move={handleMove} stop={(e)=>{handleStop(e)}} stickSize={50} ></Joystick>
                </div>
                <div className='text-white text-7xl text-center'>
                    s: {speed} a: {angle} code: {joyStickCode}
                </div>
                <button 
                    className={`mt-10 bg-blue-500 text-white font-bold text-3xl p-2 rounded-md mx-auto ${isBuzzerSpinning ? 'animate-spin' : ''}`} 
                    onClick={handleBuzzer}
                >
                    Buzzer
                </button>
            </div>
        </>
    )
}

export default App
