import Vue from 'vue'
//import App from './App.vue'
import {BootstrapVue, IconsPlugin} from 'bootstrap-vue'
import VueRouter from 'vue-router'

// Import Bootstrap an BootstrapVue CSS files (order is important)
import 'bootstrap/dist/css/bootstrap.css'
import 'bootstrap-vue/dist/bootstrap-vue.css'
import Reservation from "@/Pages/Reservation";
import GPS from "@/Pages/GPS";
import App from "@/App";
import Home from "@/Pages/Home";

Vue.config.productionTip = false

const routes = [
    {path: '/', component: Home},
    {path: '/reservation', component: Reservation},
    {path: '/gps', component: GPS}
]

const router = new VueRouter({
    base:'IoTGroupProject',
    routes,
})

Vue.use(VueRouter)
Vue.use(BootstrapVue) // Make BootstrapVue available throughout your project
Vue.use(IconsPlugin) // Optionally install the BootstrapVue icon components plugin

new Vue({
    el: "#app",
    router,
    render: h => h(App)
}).$mount('#app')


