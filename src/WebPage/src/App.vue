<template>
  <div id="app">
    <Header></Header>
    <b-container>
      <router-view style="padding: 20px;"/>
    </b-container>
  </div>
</template>

<script>
import Header from "@/components/Header";

export default {
  name: 'app',
  components: {
    Header
  }
}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
}
</style>
