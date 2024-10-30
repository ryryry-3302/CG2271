import axios from 'axios';

const BASE_URL = 'http://192.168.27.189'; // Replace with your ESP32 server IP

let lastCallTime = 0;
const THROTTLE_INTERVAL = 200; // 100ms throttle

const throttle = (func) => {
    return (...args) => {
        const now = Date.now();
        if (now - lastCallTime < THROTTLE_INTERVAL) {
            return false;
        }
        lastCallTime = now;
        return func(...args);
    };
};

// export const sendCoordinates = throttle(async (x, y, serverIP) => {
//     const response = await axios.get(`${serverIP}/x=${x}/y=${y}`);
//     return response.data;
// });

export const sendCode = throttle(async (code, serverIP) => {
    await axios.post(`http://${serverIP}/?code=${code}`);
    return code;
});

export const sendBuzzer = async (serverIP) => {
    await axios.post(`http://${serverIP}/${"buzzer"}`);
    return true;
};