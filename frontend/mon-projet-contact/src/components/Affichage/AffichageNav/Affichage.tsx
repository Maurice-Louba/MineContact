import { useNavigate } from "react-router"

const Affichage = () => {
    const navigate = useNavigate()
  return (
    <div className="mx-auto">
        <nav className="flex justify-between items-center p-6 max-w-6xl mx-auto">
        <div className="text-2xl font-bold text-indigo-600 flex absolute mx-[-210px] ">Minecontacts</div>
        <ul className="flex gap-6 text-gray-700 font-medium ">
          <li  className='hover:cursor-pointer hover:text-indigo-600'  onClick={()=>navigate('/')}>Acceuil</li>
          <li>Fonctionnalités</li>
          <li className='hover:cursor-pointer hover:text-indigo-600' onClick={()=>navigate('/contact/')}>Contacts</li>
          <li>Paramètre</li>   
          
        </ul>
        <div className="flex gap-4 mx-4">
          <button className="text-gray-700 font-medium hover:border hover:border-blue-700 hover:rounded-full   hover:p-2 transition duration-100 ">Mon profil</button>
          <button className="bg-blue-600 text-white px-4 py-2 rounded-full shadow-md hover:bg-blue-700">
            Démarrer
          </button>
        </div>
      </nav>
    </div>
  )
}

export default Affichage
