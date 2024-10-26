import { Link } from "react-router-dom"; // Import Link from react-router-dom
import { ChevronRightIcon } from "@heroicons/react/24/outline";
import { useEffect, useState } from "react";

function HoveredSubMenuItem({ icon, text, active }) {
  return (
    <div
      className={`my-2 rounded-md p-2 ${
        active ? "bg-gray-300" : "hover:bg-main"
      }`}
    >
      <div className="flex items-center justify-center">
        <span className="text-black h-6 w-6">{icon}</span>
        <span className="text-gray-600 ml-3 w-28 text-start">{text}</span>
        <div className="bg-gray-600 h-1" />
      </div>
    </div>
  );
}

export default function SidebarItem({
  icon,
  active = false,
  text,
  expanded = false,
  subMenu = null,
  to,
}) {
  const [expandSubMenu, setExpandSubMenu] = useState(false);

  useEffect(() => {
    if (!expanded) {
      setExpandSubMenu(false);
    }
  }, [expanded]);

  const subMenuHeight = expandSubMenu
    ? `${((subMenu?.length || 0) * 40 + (subMenu && 15)).toString()}px`
    : 0;

  return (
    <>
      <li>
        <Link to={to}>
          {" "}
          <button
            className={`
              group relative my-1 flex w-full cursor-pointer
              items-center rounded-md px-3
              py-2 font-medium
              ${
                active && !subMenu
                  ? "bg-gradient-to-tr from-main to-purple-400"
                  : "text-gray-600 hover:bg-purple-50"
              }
              ${!expanded && "hidden sm:flex"}
            `}
            onClick={() => setExpandSubMenu((curr) => expanded && !curr)}
          >
            <span className={`h-6 w-6 ${active ? "text-white" : ""}`}>
              {icon}
            </span>
            <span
              className={`overflow-hidden text-start transition-all ${
                expanded ? "ml-3 w-44" : "w-0"
              }
              ${active ? "text-white" : "text-black"}
              `}
            >
              {text}
            </span>
            {subMenu && (
              <div
                className={`absolute right-2 h-4 w-4${
                  expanded ? "" : "top-2"
                } transition-all ${expandSubMenu ? "rotate-90" : "rotate-0"}`}
              >
                <ChevronRightIcon />
              </div>
            )}
            {!expanded && (
              <div
                className={`
                  text-primary-500 invisible absolute left-full ml-6 -translate-x-3
                  rounded-md bg-purple-100 px-2
                  py-1 text-sm opacity-20 transition-all
                  group-hover:visible group-hover:translate-x-0 group-hover:opacity-100
                `}
              >
                {!subMenu
                  ? text
                  : subMenu.map((item, index) => (
                      <HoveredSubMenuItem
                        key={index}
                        text={item.text}
                        icon={item.icon}
                      />
                    ))}
              </div>
            )}
          </button>
        </Link>
      </li>
      <ul className="sub-menu pl-6" style={{ height: subMenuHeight }}>
        {expanded &&
          subMenu?.map((item, index) => (
            <SidebarItem key={index} {...item} expanded={expanded} />
          ))}
      </ul>
    </>
  );
}
