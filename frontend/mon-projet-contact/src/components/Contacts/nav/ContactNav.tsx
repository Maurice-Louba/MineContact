import img from '../../../assets/work-4997565_1280-removebg-preview.png'
import { useNavigate } from "react-router"
import { ReactTyped } from "react-typed"
const ContactNav = () => {
    const navigate = useNavigate()
  return (
    <div>
       <div className=" min-h-screen w-full bg-gradient-to-br from-green-100 via-white to-blue-100 flex flex-col">
      {/* Navbar */}
      <nav className="flex justify-between items-center p-6 max-w-6xl mx-auto">
        <div className="text-2xl font-bold text-indigo-600 flex absolute mx-[-210px] ">Minecontacts</div>
        <ul className="flex gap-6 text-gray-700 font-medium ">
          <li className="hover:cursor-pointer"   onClick={()=>navigate('/')}>Acceuil</li>
          <li>Fonctionnalités</li>
          <li className='hover:cursor-pointer hover:text-indigo-600' onClick={()=>navigate('/contact/')}>Contacts</li>
          <li>Paramètre</li>   
          
        </ul>
        <div className="flex gap-4 mx-4">
          <button className="text-gray-700 font-medium hover:border hover:border-blue-700 hover:rounded-full   hover:p-2 transition duration-100 ">Se connecter</button>
          <button className="bg-blue-600 text-white px-4 py-2 rounded-full shadow-md hover:bg-blue-700" onClick={()=>navigate('/contact/Formulaire/')}>
            Ajouter +
          </button>
        </div>
      </nav>
        <section className="flex flex-col-reverse md:flex-row items-center justify-between p-8 md:p-16 max-w-6xl mx-auto">
            <div className="flex-1 space-y-6">
          <h1 className="text-3xl md:text-5xl font-bold">
             <span className="text-blue-600">Bienvenue </span><br/>
            sur votre espace<br/><span className="text-indigo-600"><ReactTyped 
                      
                      strings={["de contact !", "d'amitié !","de gestion 👋"]} typeSpeed={50} backSpeed={50} loop={true} /></span>
          </h1>
          <p className="text-gray-600 text-lg">
            Centralisez, organisez et accédez à tous vos contacts professionnels et personnels depuis une seule interface claire et intuitive. Gagnez du temps et restez connecté avec ceux qui comptent.
          </p>

          <div className="flex gap-4 flex-wrap">
            <button className="bg-gradient-to-r w-[200px] from-blue-500 to-purple-600 text-white px-6 py-3  rounded-full shadow-lg hover:opacity-90" onClick={()=>navigate('Formulaire/')}>
              Ajouter +
            </button>
            <button className="bg-white border w-[200px] border-blue-300 text-blue-600 px-6 py-3 rounded-full hover:bg-blue-50" onClick={()=>navigate('liste')}>
              Voir les contacts →
            </button>
          </div>
        </div>
        <div>
            <img src={img}/>
        </div>
        </section>
      
    </div>
    </div>
  )
}

export default ContactNav
