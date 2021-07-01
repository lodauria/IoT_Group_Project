<template>
  <div>
    <b-navbar class="topBar" type="light" variant="light">
      <b-navbar-nav>
        <router-link to="/" custom v-slot={navigate}>
          <b-navbar-brand @click="navigate" @keypress.enter="navigate" role="link">Home</b-navbar-brand>
        </router-link>

        <router-link to="/reservation" custom v-slot={navigate}>
          <b-navbar-brand @click="navigate" @keypress.enter="navigate" role="link">Reservation</b-navbar-brand>
        </router-link>

        <router-link to="/gps" custom v-slot={navigate}>
          <b-navbar-brand @click="navigate" @keypress.enter="navigate" role="link">GPS</b-navbar-brand>
        </router-link>

      </b-navbar-nav>
    </b-navbar>
  </div>
</template>

<script>
export default {
  name: "Header"
}
</script>

<style scoped>
.topBar {
  padding-left: 20px;
  padding-right: 20px;
}
</style>