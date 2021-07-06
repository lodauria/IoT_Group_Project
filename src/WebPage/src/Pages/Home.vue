<template>
  <div>
    <h2>
      Smart signage for marinas
    </h2>
    <p>
      Going to the <router-link to="/reservation">page reservation</router-link> you can get a reservation for a docking spot in the marina
    </p>
    <p>
      If when you enter in the marina and you boat is not detected by the camera, you should open the page <router-link to="/gps">GPS</router-link>
      , insert you data and the system will show up the signage for your booked dock.
    </p>
    <p>
      <GithubButton href="https://github.com/kernel-machine/IoTGroupProject" data-size="large">
        Open on GitHub
      </GithubButton>
    </p>
  </div>
</template>

<script>
import GithubButton from 'vue-github-button'

export default {
  name: "Home",
  components: {
    GithubButton
  }
}
</script>

<style scoped>

</style>