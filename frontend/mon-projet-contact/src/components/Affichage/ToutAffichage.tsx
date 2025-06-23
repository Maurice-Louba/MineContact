import Footer from "../Accueil/footer"
import Affichage from "./AffichageNav/Affichage"
import ListeContacts from "./Les contacts/AffichangedesContacts"


const ToutAffichage = () => {
  return (
    <div className=" min-h-screen w-full bg-gradient-to-br from-green-100 via-white to-blue-100 flex flex-col">
      <Affichage/>
      <ListeContacts/>
      <Footer/>
    </div>
  )
}

export default ToutAffichage
