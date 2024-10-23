import axios from 'axios';

const BASE_URL = 'http://192.168.27.189'; // Replace with your ESP32 server IP

export const sendCommand = async (command) => {
  let attempts = 0;
  const maxAttempts = 10;
  while (attempts < maxAttempts) {
    try {
      const response = await axios.get(`${BASE_URL}/${command}`);
      return response.data;
    } catch (error) {
      console.error(`Error sending command (attempt ${attempts + 1}):`, error);
      attempts++;
      if (attempts === maxAttempts) {
        throw error;
      }
      // Wait for a short time before retrying
      await new Promise(resolve => setTimeout(resolve, 1000));
    }
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

export const stopMoving = () => {
  console.log('Stopping movement');
  return sendCommand('stopmoving');
};

export const stopTurning = () => {
  console.log('Stopping turning');
  return sendCommand('stopturning');
};

export const stop = () => {
  console.log('Stopping all movement');
  return Promise.all([stopMoving(), stopTurning()]);
};
