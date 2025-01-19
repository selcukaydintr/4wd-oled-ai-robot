import axios from 'axios';

// API timeout ve retry konfigürasyonu
const axiosInstance = axios.create({
    timeout: 5000,
    retries: 3,
    retryDelay: 1000
});

// İstek interceptor'ı
axiosInstance.interceptors.request.use(
    config => {
        // İstek başına token ekle
        config.headers.Authorization = `Bearer ${localStorage.getItem('token')}`;
        return config;
    },
    error => Promise.reject(error)
);

// Yanıt interceptor'ı
axiosInstance.interceptors.response.use(
    response => response,
    async error => {
        const { config, response } = error;
        
        // Retry logic
        if (!config || !config.retry) return Promise.reject(error);
        
        config.retry -= 1;
        if (config.retry === 0) {
            return Promise.reject(error);
        }
        
        // Retry delay
        await new Promise(resolve => setTimeout(resolve, config.retryDelay));
        
        return axiosInstance(config);
    }
);

export default {
    // Robot tasarım ayarları
    async saveRobotConfig(config) {
        try {
            const response = await axiosInstance.post('/api/robot/config', config);
            return response.data;
        } catch (error) {
            console.error('Config kaydetme hatası:', error);
            throw error;
        }
    },
    
    // Sensör yönetimi
    getSensorData() {
        return axiosInstance.get('/api/sensors/data');
    },
    
    // Motor kontrolü
    sendMotorCommand(command) {
        return axiosInstance.post('/api/motors/command', command);
    },
    
    // SLAM ayarları
    updateSlamConfig(config) {
        return axiosInstance.post('/api/slam/config', config);
    }
} 