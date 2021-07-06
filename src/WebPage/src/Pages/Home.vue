<template>
  <div>
    <h2>
      Smart signage system web inerface
    </h2>
	<p>
      This web page is the simplest way to interact with the smart signage system used inside the marina.
    </p>
    <p>
      Open the <router-link to="/reservation">page reservation</router-link> to make a reservation for a docking spot in the marina.
    </p>
    <p>
      If you already made a reservation and you want to use the GPS of your device to let us know when you are entering in the marina,  
	  open the page <router-link to="/gps">GPS</router-link> and follow the instructions.
    </p>
	<p>
	For more technical details about this system visit our GitHub repository.
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