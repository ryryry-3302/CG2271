import axios from 'axios';

const BASE_URL = 'http://192.168.27.189'; // Replace with your ESP32 server IP

export const sendCommand = async (command) => {
  try {
    const response = await axios.get(`${BASE_URL}/${command}`);
    return response.data;
  } catch (error) {
    console.error('Error sending command:', error);
    throw error;
  }
};

export const moveForward = () => {
  console.log('Moving forward');
  return sendCommand('forward');
};

export const moveBackward = () => {
  console.log('Moving backward');
  return sendCommand('backward');
};

export const turnLeft = () => {
  console.log('Turning left');
  return sendCommand('steerleft');
};

export const turnRight = () => {
  console.log('Turning right');
  return sendCommand('steerright');
};

export const stop = () => {
  console.log('Stopping');
  return sendCommand('stop');
};
