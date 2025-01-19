import { createStore } from 'vuex';
import robotApi from '../api/robotApi';

export default createStore({
    state: {
        robotConfig: null,
        sensorData: [],
        slamConfig: null,
        motorStatus: {},
        errors: [],
        isLoading: false
    },
    
    mutations: {
        SET_LOADING(state, status) {
            state.isLoading = status;
        },
        
        ADD_ERROR(state, error) {
            state.errors.push({
                message: error.message,
                timestamp: new Date(),
                id: Date.now()
            });
        },
        
        CLEAR_ERRORS(state) {
            state.errors = [];
        }
    },
    
    actions: {
        async saveRobotConfig({ commit }, config) {
            commit('SET_LOADING', true);
            try {
                const response = await robotApi.saveRobotConfig(config);
                commit('SET_ROBOT_CONFIG', response.data);
            } catch (error) {
                commit('ADD_ERROR', error);
                throw error;
            } finally {
                commit('SET_LOADING', false);
            }
        }
    }
}); 