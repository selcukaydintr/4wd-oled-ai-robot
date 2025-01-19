import { createStore } from 'vuex'

export default createStore({
  state: {
    // Uygulama genelinde paylaşılan veriler
    robotConfig: null,
    sensorData: {},
    motorStatus: {},
    isConnected: false,
    errors: []
  },
  
  mutations: {
    // State'i değiştiren fonksiyonlar
    SET_ROBOT_CONFIG(state, config) {
      state.robotConfig = config
    },
    UPDATE_SENSOR_DATA(state, data) {
      state.sensorData = { ...state.sensorData, ...data }
    },
    SET_MOTOR_STATUS(state, status) {
      state.motorStatus = status
    },
    SET_CONNECTION_STATUS(state, status) {
      state.isConnected = status
    },
    ADD_ERROR(state, error) {
      state.errors.push(error)
    }
  },
  
  actions: {
    // API çağrıları ve karmaşık işlemler
    async initializeRobot({ commit }) {
      try {
        const config = await fetch('/api/robot/config').then(r => r.json())
        commit('SET_ROBOT_CONFIG', config)
        commit('SET_CONNECTION_STATUS', true)
      } catch (error) {
        commit('ADD_ERROR', error)
      }
    },
    
    async updateSensorData({ commit }) {
      try {
        const data = await fetch('/api/sensors/data').then(r => r.json())
        commit('UPDATE_SENSOR_DATA', data)
      } catch (error) {
        commit('ADD_ERROR', error)
      }
    }
  },
  
  getters: {
    // Hesaplanmış değerler
    isRobotReady: state => {
      return state.isConnected && state.robotConfig !== null
    },
    getSensorValue: state => (sensorId) => {
      return state.sensorData[sensorId] || null
    }
  }
}) 