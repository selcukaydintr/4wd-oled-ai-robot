import { createRouter, createWebHistory } from 'vue-router'
import Dashboard from '@/views/Dashboard.vue'
import MotorDashboard from '@/views/MotorDashboard.vue'
import SensorConfig from '@/views/SensorConfig.vue'
import RobotDesign from '@/views/RobotDesign.vue'
import SLAM from '@/views/SLAM.vue'

const routes = [
  {
    path: '/',
    name: 'Dashboard',
    component: Dashboard
  },
  {
    path: '/motors',
    name: 'MotorDashboard',
    component: MotorDashboard
  },
  {
    path: '/sensors',
    name: 'SensorConfig',
    component: SensorConfig,
    // Alt rotalar
    children: [
      {
        path: 'ultrasonic',
        component: () => import('@/views/sensors/UltrasonicConfig.vue')
      },
      {
        path: 'imu',
        component: () => import('@/views/sensors/IMUConfig.vue')
      }
    ]
  },
  {
    path: '/design',
    name: 'RobotDesign',
    component: RobotDesign
  },
  {
    path: '/slam',
    name: 'SLAM',
    component: SLAM
  }
]

const router = createRouter({
  history: createWebHistory(),
  routes
})

// Gezinme koruması
router.beforeEach((to, from, next) => {
  // Örnek: Robot bağlı değilse bazı sayfalara erişimi engelle
  const store = useStore()
  if (to.name !== 'Dashboard' && !store.getters.isRobotReady) {
    next({ name: 'Dashboard' })
  } else {
    next()
  }
})

export default router 