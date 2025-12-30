import { createRouter, createWebHistory } from 'vue-router'

import Home from '../pages/Home.vue'
import Health from '../pages/Health.vue'
import Lidar from '../pages/Lidar.vue'
import Camera from '../pages/Camera.vue'

export default createRouter({
  history: createWebHistory(),
  routes: [
    { path: '/', component: Home },
    { path: '/health', component: Health },
    { path: '/lidar', component: Lidar},
    { path: '/camera', component: Camera}
  ]
})

