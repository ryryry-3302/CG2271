import axios from 'axios';

const BASE_URL = 'http://192.168.27.189'; // Replace with your ESP32 server IP

let lastCallTime = 0;
const THROTTLE_INTERVAL = 100; // 100ms throttle

const throttle = (func) => {
    return (...args) => {
        const now = Date.now();
        if (now - lastCallTime < THROTTLE_INTERVAL) {
            return;
        }
        lastCallTime = now;
        return func(...args);
    };
};

export const sendCoordinates = throttle(async (x, y, serverIP) => {
    const response = await axios.get(`${serverIP}/x=${x}/y=${y}`);
    return response.data;
});

export const sendBuzzer = throttle(async (serverIP) => {
    const response = await axios.get(`${serverIP}/${"buzzer"}`);
    return response.data;
});