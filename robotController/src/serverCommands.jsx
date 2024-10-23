import axios from 'axios';

const BASE_URL = 'http://192.168.27.189'; // Replace with your ESP32 server IP

export const sendCoordinates = async (x, y) => {
    const response = await axios.get(`${BASE_URL}/x=${x}/y=${y}`);
    return response.data;
};

export const sendBuzzer = async () => {
    const response = await axios.get(`${BASE_URL}/${"buzzer"}`);
    return response.data;
};