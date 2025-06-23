import { useEffect, useState, useRef } from "react";
import photo from '../../../assets/blind-1027860_1280-removebg-preview.png'

interface Contact {
  nom: string;
  tel: string;
  mail: string;
}

export default function ListeContacts() {
  const [contacts, setContacts] = useState<Contact[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [search, setSearch] = useState<string>("");

  const [menuOuvertIndex, setMenuOuvertIndex] = useState<number | null>(null);
  const [contactDetail, setContactDetail] = useState<Contact | null>(null);
  const menuRef = useRef<HTMLDivElement>(null);

  useEffect(() => {
    fetchContacts();
  }, []);

  useEffect(() => {
    function handleClickOutside(event: MouseEvent) {
      if (menuRef.current && !menuRef.current.contains(event.target as Node)) {
        setMenuOuvertIndex(null);
      }
    }
    if (menuOuvertIndex !== null) {
      document.addEventListener("mousedown", handleClickOutside);
    } else {
      document.removeEventListener("mousedown", handleClickOutside);
    }
    return () => document.removeEventListener("mousedown", handleClickOutside);
  }, [menuOuvertIndex]);

  function fetchContacts() {
    setLoading(true);
    setError(null);
    fetch("http://localhost:8888/contacts")
      .then((res) => {
        if (!res.ok) throw new Error("Erreur lors du chargement des contacts");
        return res.json();
      })
      .then((data: Contact[]) => {
        setContacts(data);
        setLoading(false);
      })
      .catch((err) => {
        setError(err.message);
        setLoading(false);
      });
  }

  function handleSupprimer(idx: number) {
    const contact = contacts[idx];
    if (!window.confirm(`Supprimer le contact ${contact.nom} ?`)) return;

    fetch("http://localhost:8888/contacts", {
      method: "DELETE",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ nom: contact.nom, tel: contact.tel }),
    })
      .then((res) => {
        if (!res.ok) throw new Error("Erreur lors de la suppression");
        fetchContacts();
        setMenuOuvertIndex(null);
      })
      .catch((err) => {
        alert(err.message);
      });
  }

  const filteredContacts = contacts.filter((c) =>
    c.nom.toLowerCase().includes(search.toLowerCase())
  );

  if (loading)
    return (
      <p className="text-center text-gray-500 mt-10 text-lg">Chargement des contacts...</p>
    );
  if (error)
    return (
      <p className="text-center text-red-600 mt-10 text-lg">Erreur : {error}</p>
    );
  if (contacts.length === 0)
    return (
      <div>
        <p className="text-center text-gray-600 mt-10 text-lg font-medium">Aucun contact inscrit.</p>
        <img src={photo} className="mx-auto px-8" />
      </div>
    );

  return (
    <div className="max-w-4xl mx-auto p-6">
      <h2 className="text-3xl font-bold mb-6 text-indigo-600 text-center">Liste des contacts inscrits</h2>

      <input
        type="text"
        placeholder="Rechercher un nom..."
        value={search}
        onChange={(e) => setSearch(e.target.value)}
        className="w-full mb-6 p-3 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-indigo-400"
      />

      <div className="overflow-x-auto shadow-lg rounded-lg border border-gray-200">
        <table className="min-w-full bg-white relative">
          <thead>
            <tr>
              <th className="py-3 px-6 bg-indigo-100 text-left text-indigo-700 font-semibold">Nom</th>
              <th className="py-3 px-6 bg-indigo-100 text-left text-indigo-700 font-semibold">Téléphone</th>
              <th className="py-3 px-6 bg-indigo-100 text-left text-indigo-700 font-semibold">E-mail</th>
              <th className="py-3 px-6 bg-indigo-100"></th>
            </tr>
          </thead>
          <tbody>
            {filteredContacts.map((c, idx) => (
              <tr key={idx} className={idx % 2 === 0 ? "bg-white" : "bg-indigo-50"}>
                <td className="py-3 px-6 border-b border-gray-200">{c.nom}</td>
                <td className="py-3 px-6 border-b border-gray-200">{c.tel}</td>
                <td className="py-3 px-6 border-b border-gray-200">{c.mail}</td>
                <td className="py-3 px-6 border-b border-gray-200 relative">
                  <button
                    className="p-2 rounded hover:bg-gray-200"
                    onClick={() =>
                      setMenuOuvertIndex(menuOuvertIndex === idx ? null : idx)
                    }
                    aria-label="Menu options"
                  >
                    <svg
                      xmlns="http://www.w3.org/2000/svg"
                      className="h-6 w-6 text-gray-600"
                      fill="none"
                      viewBox="0 0 24 24"
                      stroke="currentColor"
                      strokeWidth={2}
                    >
                      <path strokeLinecap="round" strokeLinejoin="round" d="M12 6v.01M12 12v.01M12 18v.01" />
                    </svg>
                  </button>

                  {menuOuvertIndex === idx && (
                    <div
                      ref={menuRef}
                      className="absolute right-0 mt-2 w-40 bg-white border border-gray-300 rounded shadow-lg z-50"
                    >
                      <button
                        className="block w-full text-left px-4 py-2 hover:bg-indigo-100"
                        onClick={() => {
                          setContactDetail(c);
                          setMenuOuvertIndex(null);
                        }}
                      >
                        Voir
                      </button>
                      <button
                        className="block w-full text-left px-4 py-2 hover:bg-red-100 text-red-600"
                        onClick={() => handleSupprimer(idx)}
                      >
                        Supprimer
                      </button>
                      <button
                        className="block w-full text-left px-4 py-2 hover:bg-gray-100"
                        onClick={() => setMenuOuvertIndex(null)}
                      >
                        Paramètres
                      </button>
                    </div>
                  )}
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>

      {contactDetail && (
        <div
          className="fixed inset-0 bg-black bg-opacity-40 backdrop-blur-sm flex justify-center items-center z-50"
          onClick={() => setContactDetail(null)}
        >
          <div
            className="bg-white rounded-lg p-6 max-w-md w-full shadow-lg"
            onClick={(e) => e.stopPropagation()}
          >
            <h3 className="text-2xl font-bold mb-4">Détails du contact</h3>
            <p><strong>Nom:</strong> {contactDetail.nom}</p>
            <p><strong>Téléphone:</strong> {contactDetail.tel}</p>
            <p><strong>E-mail:</strong> {contactDetail.mail}</p>
            <button
              className="mt-6 bg-indigo-600 text-white px-4 py-2 rounded hover:bg-indigo-700"
              onClick={() => setContactDetail(null)}
            >
              Fermer
            </button>
          </div>
        </div>
      )}
    </div>
  );
}
